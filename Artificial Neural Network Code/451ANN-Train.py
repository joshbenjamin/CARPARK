'''

  This program is used to generate weights to be used in an ANN

'''

import numpy as np
import sys

# This is a list of selected values that is used to train the ANN on inputs and expected outputs
# (These points were gathered from field testing and observing the distance that is picked up from the Ultrasonic sensor)
xAll = np.array(([2241, 2234, 2236, 2242],
                 [2239, 2256, 76, 2251],
                 [82, 2241, 2245, 2256],
                 [2242, 79, 2245, 2256],
                 [2234, 2241, 2245, 124],
                 [81, 2256, 76, 2251],
                 [82, 2241, 2245, 92],
                 [2242, 79, 2245, 83],
                 [2239, 82, 76, 2251],
                 [82, 91, 2245, 2256],
                 [2234, 2241, 109, 124],
                 [2240, 2241, 78, 69],
                 [67, 78, 71, 2241],
                 [2239, 109, 113, 119],
                 [2251, 64, 56, 55],
                 [120, 116, 120, 2232],
                 [76, 78, 81, 85],
                 [80, 80, 80, 80],
                 [102, 105, 91, 100],
                 [2242, 112, 104, 94]),
                dtype=float)

# The expected outputs
y = np.array(([0],
              [0],
              [0],
              [0],
              [0],
              [0],
              [0],
              [0],
              [1],
              [1],
              [1],
              [1],
              [1],
              [1],
              [1],
              [1],
              [1],
              [1],
              [1]),
             dtype=float)

# Scales the input data by the maximum observed value of 2261
xAll = xAll/[2261, 2261, 2261, 2261]

# Splits the input data into the ones that are known and one set that is not known
X = np.split(xAll, [19])[0]
# Becomrs the predicting data
xPredicted = np.split(xAll, [19])[1]

class Neural_Network(object):
  def __init__(self):

    # Parameters for the shape of the ANN
    self.inputSize = 4
    self.outputSize = 1
    self.hiddenSize = 5

    # Array of weights between each layer
    self.W1 = np.random.randn(self.inputSize, self.hiddenSize) # (4x5) weight matrix from input to hidden layer
    self.W2 = np.random.randn(self.hiddenSize, self.outputSize) # (5x1) weight matrix from hidden to output layer

  def forward(self, X):
    #forward propagation through our network
    self.z = np.dot(X, self.W1) # dot product of X (input) and first set of 3x2 weights
    self.z2 = self.sigmoid(self.z) # activation function
    self.z3 = np.dot(self.z2, self.W2) # dot product of hidden layer (z2) and second set of 3x1 weights
    o = self.sigmoid(self.z3) # final activation function
    return o

  def sigmoid(self, s):
    # activation function
    return 1/(1+np.exp(-s))

  def sigmoidPrime(self, s):
    #derivative of sigmoid
    return s * (1 - s)

  def backward(self, X, y, o):
    # backward propagate through the network
    self.o_error = y - o # error in output
    self.o_delta = self.o_error*self.sigmoidPrime(o) # applying derivative of sigmoid to error

    self.z2_error = self.o_delta.dot(self.W2.T) # z2 error: how much our hidden layer weights contributed to output error
    self.z2_delta = self.z2_error*self.sigmoidPrime(self.z2) # applying derivative of sigmoid to z2 error

    self.W1 += X.T.dot(self.z2_delta) # adjusting first set (input --> hidden) weights
    self.W2 += self.z2.T.dot(self.o_delta) # adjusting second set (hidden --> output) weights

  # A function run iteratively to train the ANN by doing a feed forward and then backpropagating
  def train(self, X, y):
    o = self.forward(X)
    self.backward(X, y, o)

  # Saves the weights to 2 files
  def saveWeights(self):
    np.savetxt("w1.txt", self.W1, fmt="%s", delimiter=',', newline='\n')
    np.savetxt("w2.txt", self.W2, fmt="%s", delimiter=',', newline='\n')

  # Makes a prediction using the weights that are created after iterations, on a set of data that has been given
  def predict(self):
    print("Predicted data based on trained weights: ")
    print("Input (scaled): \n" + str(xPredicted))
    print("Output: \n" + str(self.forward(xPredicted)))


if __name__ == '__main__':

    # Takes the number of iterations from a command-line argument
    number_of_iterations = int(sys.argv[1])

    NN = Neural_Network()

    # Trains the ANN for a number of iterations
    for i in range(number_of_iterations):

        # This will always be true. Was used at an earlier stage to test
        if (i > -1):
            
            # Prints the index of the training iteration with it's Mean Squared Error
            print(str(i+1) + "," + str(100 * np.mean(np.square(y - NN.forward(X)))))

        # Train the ANN at 1 iterations
        NN.train(X, y)

    # Methods to call once the ANN has been run over its iterations
    NN.saveWeights()
    NN.predict()