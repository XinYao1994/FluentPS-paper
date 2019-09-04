'''Train CIFAR10 with PyTorch.'''
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import torch.backends.cudnn as cudnn

import torchvision
import torchvision.transforms as transforms

import os
import argparse


parser = argparse.ArgumentParser(description='PyTorch CIFAR10 Training')
parser.add_argument('--lr', default=0.1, type=float, help='learning rate')
parser.add_argument('--resume', '-r', action='store_true', help='resume from checkpoint')
args = parser.parse_args()

device = 'cuda' if torch.cuda.is_available() else 'cpu'
best_acc = 0  # best test accuracy
start_epoch = 0  # start from epoch 0 or last checkpoint epoch

# Data
print('==> Preparing data..')
transform_train = transforms.Compose([
    transforms.RandomCrop(32, padding=4),
    transforms.RandomHorizontalFlip(),
    transforms.ToTensor(),
    transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
])

transform_test = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.4914, 0.4822, 0.4465), (0.2023, 0.1994, 0.2010)),
])

trainset = torchvision.datasets.CIFAR10(root='./data', train=True, download=True, transform=transform_train)
trainloader = torch.utils.data.DataLoader(trainset, batch_size=32, shuffle=True, num_workers=1)

testset = torchvision.datasets.CIFAR10(root='./data', train=False, download=True, transform=transform_test)
testloader = torch.utils.data.DataLoader(testset, batch_size=32, shuffle=False, num_workers=1)

classes = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck')

# Model
print('==> Building model..')
# net = VGG('VGG19')
# net = ResNet18()
# net = PreActResNet18()
# net = GoogLeNet()
# net = DenseNet121()
# net = ResNeXt29_2x64d()
# net = MobileNet()
# net = MobileNetV2()
# net = DPN92()
# net = ShuffleNetG2()
# net = SENet18()
# net = ShuffleNetV2(1)
# net = EfficientNetB0()
net = torchvision.models.resnet50(pretrained=True)

# design ps architecture
import sys
''' import fps package '''
import fps.wrapper as fps

from multiprocessing import Process

''' rewrite run.sh in it, using os.environ '''

num_servers = 1
num_workers = 2

train_internal = 100
test_internal = 100
Iterations = 64000

if len(sys.argv) > 1:
    num_servers = int(sys.argv[1])
    
if len(sys.argv) > 2:
    num_servers = int(sys.argv[2])

os.environ['DMLC_NUM_SERVER'] = str(num_servers)
os.environ['DMLC_NUM_WORKER'] = str(num_workers)

os.environ['DMLC_PS_ROOT_URI'] = '127.0.0.1'
os.environ['DMLC_PS_ROOT_PORT'] = '8000'

os.environ['DMLC_NODE_HOST'] = '127.0.0.1'
os.environ['DMLC_NODE_VHOST'] = '127.0.0.1'

criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(net.parameters(), lr=args.lr, momentum=0.9, weight_decay=5e-4)

def install_scheduler(name):
    print('Run scheduler task %s (%s)...' % (name, os.getpid()))
    os.environ['DMLC_ROLE'] = 'scheduler'
    c = fps.scheduler()
    c.Init(0)
    c.Finalize(0)

    
def install_server(name):
    print('Run server task %s (%s)...' % (name, os.getpid()))
    os.environ['DMLC_ROLE'] = 'server'
    os.environ['HEAPPROFILE'] = './S' + str(name)
    s = fps.server()
    s.Init(0)
    s.create(0)
    s.start()
    s.Finalize(0)

def test_all():
    net.eval()
    test_loss = 0
    correct = 0
    total = 0
    with torch.no_grad():
        for batch_idx, (inputs, targets) in enumerate(testloader):
            inputs, targets = inputs.to(device), targets.to(device)
            outputs = net(inputs)
            loss = criterion(outputs, targets)

            test_loss += loss.item()
            _, predicted = outputs.max(1)
            total += targets.size(0)
            correct += predicted.eq(targets).sum().item()

    acc = 100.*correct/total
    print("Loss: %.3f | Acc: %.3f %d %d"% (test_loss/(batch_idx+1), 100.*correct/total, correct, total))
    
def install_worker(name):
    print('Run worker task %s (%s)...' % (name, os.getpid()))
    if name % 2 == 0: 
        os.environ['CUDA_VISIBLE_DEVICES'] = 0   # use 2080ti
    else:
        os.environ['CUDA_VISIBLE_DEVICES'] = 1   # use 2070
        
    os.environ['DMLC_ROLE'] = 'worker'
    os.environ['HEAPPROFILE'] = './W' + str(name)
    w = fps.worker()
    w.Init(0)
    w.create(0, 0)
    # w.start() we do not need to set slicer
    '''
    do somethings like push/pull
    '''
    net = net.to(device)
    if device == 'cuda':
        net = torch.nn.DataParallel(net)
        cudnn.benchmark = True
        
    iter = 0
    # currently, only one server
    keys = list(range(num_servers))
    lens = list(range(num_servers))
    # get number of parameter 
    model_parameters = filter(lambda p: p.requires_grad, net.parameters())
    trainable_count = sum([np.prod(p.size()) for p in model_parameters])
    vals_ = list(np.zeros(trainable_count))
    # vals = []
    net.train()
    train_loss = 0
    correct = 0
    total = 0
    for i in range(num_servers):
        lens[i] = trainable_count/num_servers
    
    while True:    
        for batch_idx, (inputs, targets) in enumerate(trainloader):
            inputs, targets = inputs.to(device), targets.to(device)
            optimizer.zero_grad()
            outputs = net(inputs)
            loss = criterion(outputs, targets)
            loss.backward()
            
            train_loss += loss.item()
            _, predicted = outputs.max(1)
            total += targets.size(0)
            correct += predicted.eq(targets).sum().item()
            if iter % train_internal == (train_internal-1):
                print("Train Loss: %.3f | Acc: %.3f %d %d"% (train_loss/train_internal, 100.*correct/total, correct, total))
            # write gradient to push
            p_index = 0
            vals = []
            model_parameters = filter(lambda p: p.requires_grad, net.parameters())
            for w in model_parameters:
                vals.extend(list(w.grad.data.cpu().numpy().flatten()))
            
            last = w.spush(keys, vals, iter, lens)
            w.wait(w.spull(keys, iter))
            # read parameter from pull, model.trainable_weights 
            ret = w.read()
            p_index = 0
            model_parameters = filter(lambda p: p.requires_grad, net.parameters())
            for w in model_parameters:
                shape_ = w.size()
                # w.grad
                w.grad.data = torch.from_numpy((np.array(ret[p_index:p_index+np.prod(shape_)]) - np.array(vals_[p_index:p_index+np.prod(shape_)])).reshape(shape_)).float().to(device)
                p_index = p_index+np.prod(shape_)
                # vals.extend(list(w.grad.data.cpu().numpy().flatten()))
            
            vals_ = ret
            optimizer.step()
            if iter % text_internal == (text_internal-1):
                test_all()
            
            iter = iter + 1
            
        if iter >= Iterations:
            break;
        '''
        # Save model and weights
        if not os.path.isdir(save_dir):
           os.makedirs(save_dir)
           model_path = os.path.join(save_dir, model_name)
           model.save(model_path)
        print('Saved trained model at %s ' % model_path)
        '''
    # w.wait(last)
    # w.wait(w.spull(keys, repeat - 1))
    # ret = w.read()
    # print(ret)
    w.Finalize(0)
    # Score trained model.
    # scores = model.evaluate(x_test, y_test, verbose=1)
    # print('Test loss:', scores[0])
    # print('Test accuracy:', scores[1])
    
# num_processes = 1 + num_servers + num_workers

# _pool = Pool(processes=num_processes)

# ret_scheduler = []
# ret_server = []
# ret_worker = []


p_arr = []

for i in range(1):
    p_arr.append(Process(target=install_scheduler, args=(i,)))
    # ret_scheduler.append(_pool.apply_async(install_scheduler, args=(i,)))

for i in range(num_servers):
    p_arr.append(Process(target=install_server, args=(i,)))
    # ret_server.append(_pool.apply_async(install_server, args=(i,)))

for i in range(num_workers):
    p_arr.append(Process(target=install_worker, args=(i,)))
    # ret_worker.append(_pool.apply_async(install_worker, args=(i,)))

for p in p_arr:
    p.start()
    
print('Waiting for all subprocesses done...')

for p in p_arr:
    p.join()
# _pool.close()
# _pool.join()
print('Test SSP is done.')
    
# for result in (ret_scheduler+ret_server+ret_worker):
#     print(result)

