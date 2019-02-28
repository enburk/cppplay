import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np


# https://en.wikipedia.org/wiki/Random_walk

# https://en.wikipedia.org/wiki/Rosenbrock_function

# https://en.wikipedia.org/wiki/Gambler%27s_fallacy#Monte_Carlo_Casino

# Chevalier de Méré, Blaise Pascal
# boxcars/midnight (two sixes)
def checkPascal():
    yes = 0
    trials = 10000
    in_a_row = 25
    for trial in range (trials):
        for i in range (in_a_row):
            die1 = np.random.choice([1,2,3,4,5,6])
            die2 = np.random.choice([1,2,3,4,5,6])
            if die1 == 6 and die2 == 6:
                    yes += 1
                    break
    print ('==========================')
    print ('Chevalier de Méré problem:')
    print ('==========================')
    print (trials, 'trials,', in_a_row, 'dice rolls in a row')
    print (yes,    'successful')
    print (yes / trials, '(percent of successful)')
    print (1.0 - (35/36)**in_a_row, '(true probability)')
    print ('')
### checkPascal()

class Sinusoid: # https://jakevdp.github.io/blog/2012/08/18/matplotlib-animation-tutorial/
    def __init__(self):
        self.fig = plt.figure()
        self.ax = plt.axes(xlim=(0, 2), ylim=(-2, 2))
        self.line, = self.ax.plot([], [], lw=2)

    def init(self):
        self.line.set_data([], [])
        return self.line,

    def animate(self, i):
        x = np.linspace(0, 2, 1000)
        y = np.sin(2 * np.pi * (x - 0.01 * i))
        self.line.set_data(x, y)
        return self.line,

    def show(self):
        anim = animation.FuncAnimation(self.fig, self.animate, init_func=self.init,
                                frames=200, interval=20, blit=True)
        plt.show()
Sinusoid ().show()

