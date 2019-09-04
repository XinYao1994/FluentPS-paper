from __future__ import print_function
import keras
from keras.datasets import cifar10
from keras.preprocessing.image import ImageDataGenerator
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.applications.resnet50 import ResNet50
from keras import backend as K
import os
import numpy as np

batch_size = 64
num_classes = 10
epochs = 100
Iterations = 64000

'''
save_dir = os.path.join(os.getcwd(), 'saved_models')
model_name = 'keras_cifar10_trained_model.h5'
'''

# The data, split between train and test sets:
(x_train, y_train), (x_test, y_test) = cifar10.load_data()
print('x_train shape:', x_train.shape)
print(x_train.shape[0], 'train samples')
print(x_test.shape[0], 'test samples')

# Convert class vectors to binary class matrices.
y_train = keras.utils.to_categorical(y_train, num_classes)
y_test = keras.utils.to_categorical(y_test, num_classes)

'''
model = Sequential()
model.add(Conv2D(32, (3, 3), padding='same',
                 input_shape=x_train.shape[1:]))
model.add(Activation('relu'))
model.add(Conv2D(32, (3, 3)))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))

model.add(Conv2D(64, (3, 3), padding='same'))
model.add(Activation('relu'))
model.add(Conv2D(64, (3, 3)))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.25))

model.add(Flatten())
model.add(Dense(512))
model.add(Activation('relu'))
model.add(Dropout(0.5))
model.add(Dense(num_classes))
model.add(Activation('softmax'))
'''

base_model = ResNet50(include_top=False, weights=None,
             input_shape=(32, 32, 3))

model = Sequential()
model.add(base_model)
model.add(keras.layers.Flatten())
model.add(keras.layers.BatchNormalization())
model.add(keras.layers.Dense(128, activation='relu'))
model.add(keras.layers.Dropout(0.5))
model.add(keras.layers.BatchNormalization())
model.add(keras.layers.Dense(64, activation='relu'))
model.add(keras.layers.Dropout(0.5))
model.add(keras.layers.BatchNormalization())
model.add(keras.layers.Dense(10, activation='softmax'))

# model.summary()

# initiate RMSprop optimizer
opt = keras.optimizers.rmsprop(lr=0.0001, decay=1e-6)

# Let's train the model using RMSprop
model.compile(loss='categorical_crossentropy',
              optimizer=opt,
              metrics=['accuracy'])

x_train = x_train.astype('float32')
x_test = x_test.astype('float32')
x_train /= 255
x_test /= 255

datagen = ImageDataGenerator(
    featurewise_center=False,  # set input mean to 0 over the dataset
    samplewise_center=False,  # set each sample mean to 0
    featurewise_std_normalization=False,  # divide inputs by std of the dataset
    samplewise_std_normalization=False,  # divide each input by its std
    zca_whitening=False,  # apply ZCA whitening
    zca_epsilon=1e-06,  # epsilon for ZCA whitening
    rotation_range=0,  # randomly rotate images in the range (degrees, 0 to 180)
    # randomly shift images horizontally (fraction of total width)
    width_shift_range=0.1,
    # randomly shift images vertically (fraction of total height)
    height_shift_range=0.1,
    shear_range=0.,  # set range for random shear
    zoom_range=0.,  # set range for random zoom
    channel_shift_range=0.,  # set range for random channel shifts
    # set mode for filling points outside the input boundaries
    fill_mode='nearest',
    cval=0.,  # value used for fill_mode = "constant"
    horizontal_flip=True,  # randomly flip images
    vertical_flip=False,  # randomly flip images
    # set rescaling factor (applied before any other transformation)
    rescale=None,
    # set function that will be applied on each input
    preprocessing_function=None,
    # image data format, either "channels_first" or "channels_last"
    data_format=None,
    # fraction of images reserved for validation (strictly between 0 and 1)
    validation_split=0.0)

datagen.fit(x_train)

train_data = datagen.flow(x_train, y_train, batch_size=batch_size)

trainable_count = int(np.sum([K.count_params(p) for p in set(model.trainable_weights)]))
# print the trainable_weights
trainable_parameters = [K.eval(p) for p in set(model.trainable_weights)]
print(trainable_parameters[39])

for p in set(model.trainable_weights):
    K.set_value(p, np.ones(p.shape))

trainable_parameters = [K.eval(p) for p in set(model.trainable_weights)]
print(trainable_parameters[39])

# 
# trainable_parameters = [ for p in set(model.trainable_weights)]
# print(trainable_parameters)
# print(np.array(model.trainable_weights))

exit()
# design ps architecture
import sys
''' import fps package '''
import fps.wrapper as fps

from multiprocessing import Process

''' rewrite run.sh in it, using os.environ '''

num_servers = 1
num_workers = 2

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
    iter = 0
    # currently, only one server
    keys = list(range(num_servers))
    vals = []
    lens = list(range(num_servers))
    # get number of parameter 
    trainable_count = int(np.sum([K.count_params(p) for p in set(model.trainable_weights)]))
    for i in range(num_servers):
        lens[i] = trainable_count/num_servers
    for x_train_, y_train_ in train_data:
        if iter % 100 == 0:
            print('The worker %s enter iteration %s' % (name, iter))
            model.fit(x_train_, y_train_,
                 batch_size=batch_size,
                 validation_data=(x_test, y_test))
            continue
        model.fit(x_train_, y_train_,
             batch_size=batch_size, verbose=0)
        # write gradient to push
        
        last = w.spush(keys, vals, iter, lens)
        w.wait(w.spull(keys, iter))
        # read parameter from pull, model.trainable_weights 
        ret = w.read()
        
        
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
    w.wait(last)
    w.wait(w.spull(keys, repeat - 1))
    ret = w.read()
    print(ret)
    w.Finalize(0)
    # Score trained model.
    scores = model.evaluate(x_test, y_test, verbose=1)
    print('Test loss:', scores[0])
    print('Test accuracy:', scores[1])
    
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

