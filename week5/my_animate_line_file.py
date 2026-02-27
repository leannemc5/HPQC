import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import sys
import pandas as pd
#import math as m
#import random

from pathlib import Path

def generate_path(home_folder = str(Path.home()), subfolder = '/data/', basename = 'output', extension = 'txt'):
    """This function creates the path to store the data. Note that the data is not
    stored in the directory the code is executed in. This prevents git repos
    from becoming cluttered with data that should be separate.

    Keyword Args:
        home_folder (str): the root folder in which the file is specified
            Defaults to Home
        subfolder (str): the subfolder in which the file is specified
            Defaults to /data/
        basename (str): the base of the filename to be specified
            Defaults to output
        extension (str): the file type to be specified
            Defaults to txt
    """
    # uses the method Path.home() to find the home directory in any OS
    output_folder = home_folder + subfolder  # appends a subdirectory within it.
    filename = basename + '.' + extension  # defines the filename the output is to be saved in
    output_path = output_folder + filename  # creates the output path
    return output_path

def execute_time_step(rope, y_positions, current_y_data):
    """This function carries out the update process for the animation.
    All points on the string 'follow the leader', except for point 0,
    which is 'driven' by in a simple harmonic oscillator

    Args:
        rope (list of Line2D): the plot showing the data to be updated
        y_positions (1-d NumPy array): the underlying data to be ploted
        time (float): the current time in the animation

    modified to take data from csv instead of simulating it
    """

    # updates the plotted data in the animation
    rope.set_ydata(current_y_data)
    return rope,


def reset_plots(rope, y_positions):
    """This function resets the data in the plot when the plotting
    loop is completed.

    Args:
        rope (list of Line2D): the plot showing the data to be updated
        y_positions (1-d NumPy array): the underlying data to be ploted
    """
    # Calls the initial function to set up the string
    x_positions, y_positions = configure_rope()

    # sets the data for the rope to the initial value
    rope.set_data(x_positions, y_positions)


def animate(time, rope, y_positions):
    """This function carries out the animation steps and resets the
    plots at the start of the loop if allowed to run.

    Args:
        time (float): current timestamp in the animation
        rope (list of Line2D): the plot showing the data to be updated
        y_positions (1-d NumPy array): the underlying data to be ploted

    Returns:
        rope, (tuple of (lists of Line2D)): iterable collection of the
            plots which have been animated in this function.
            Note that even if only one plot is included in the animation,
            this must still be returned as an iterable, speciied by a
            trailing comma.
    """
    # if the time has looped around to 0.
    if time == 0:
        # reset the plot and data to show the initial condition
        reset_plots(rope, y_positions)

    # execute the update operation
    execute_time_step(rope, y_positions, time)

    # return a tuple of plots (lists of Line2D)
    return rope,


def configure_animation(run_time = 5, fps = 25, end_frame=True):
    """This function sets up the variables reauored for the animations in
    Python. These are returned as a tuple which must be parsed by the
    calling process.

    Keyword Args:
        run_time (number): Intended time in seconds that the animation
            will run for. After this time, the animation will loop
            Defaults to 5 (seconds)
        fps (number): Intended number of frames per second in the
            animation. This number is returned as well.
            Defaults to 25
        end_frame (bool): used to determine if a final frame at
            time = run_time is included by adding one frame at the end
            If False, times will run from 0 to run_time - interval
            Defaults to True

    Returns:
        times (np array of floats): the time stamps (in seconds) for each
            step in the animation. In this program, these are directly
            mapped to the actual time of the animation, but this is not
            required by FuncAnimation().
        interval (int): the number of milliseconds between frames, to a
            minimum of 1ms. Used to configure the animation. In this
            program, this is matched to the framerate at which the
            animation is saved, but this is not required by ani.save()
        fps (number): the number of frames per secdond to be used in the
            final version of the animation saved to file.
    """
    # calculates the total number of frames in the animation
    frame_count = run_time * fps + end_frame

    # calculates the interval in milliseconds between frames, minimum 1
    interval = max([int(1000 / fps), 1])

    # calculates the time step in seconds to be used between frames
    time_step = interval / 1000

    # creates a numpy array of the times in the animation
    times = np.linspace(0., run_time, int(frame_count))

    # returns a tuple with the times, interval and frames per second
    return times, interval, fps


def configure_rope(length=50, end_point=True, step_size=1, start = 0):
    """Configures the parameters of the string which is to be used in
    the animation.

    Keyword Args:
        length (number): the length of the string to simulate
            Defaults to 50
        end_point (bool): whether to include a final point at
            point_labels = length.
            Defaults to True
        step_size (number): the distance between simulated points
            Defaults to 1
        start (number): the position of the start of the string.
            Defaults to 0

    Returns:
        x_positions (Numpy array of floats): initial values for the
            x-positions of the points on the string
        y_positions (Numpy array of floats): initial values for the
            y-positions of the points on the string
    """
    # calculates the number of points to simulate
    points = int((length/step_size) + end_point)

    # calculates the position of the end of the string
    stop = start + length

    # calculates the x-positions of the points on the string
    x_positions = np.linspace(start=start, stop=stop, num=points, endpoint=end_point)

    # sets the initial positions of the simulated points to zero
    y_positions = np.zeros(points)

    # returns a tuple containing the positions of the points on the string
    return x_positions, y_positions


def configure_plot(x_positions, y_positions):
    """This function sets up the plot for the animation.  It sets the
    initial x and y values, the title and the limits for the axes.

    Args:
        x_positions (Numpy array of floats): initial values for the
            x-positions of the points on the string
        y_positions (Numpy array of floats): initial values for the
            y-positions of the points on the string

    Returns:
        fig (matplotlib.figure.Figure): the figure to be animated
        rope (list of Line2D): the plot showing the data to be animated
    """
    # creates the figure and the axis to be plotted
    fig, (ax1) = plt.subplots(1, 1)

    # sets the overall title of the plot
    fig.suptitle('Simulation of Vibrations in a string')

    # sets the x and y limits to be plotted
    ax1.set_ylim(-1.1, 1.1)

    # creates the plot which will be animated
    rope, = ax1.plot(x_positions, y_positions, "o", markersize=5, color="green", label="points on string")

    # creates the legend entry
    ax1.legend(handles=[rope], bbox_to_anchor=(0, 0), loc='upper left')

    # returns the figure and the plot to be animated
    return fig, rope


def main():
    """This is the main function that executes the rest of the program
    Using a main function instead of executing directly in the global
    namespace allows for local variables and better control of scope.
    """
    # check for command line arguments
    if len(sys.argv) < 3:
        print("Usage: python3 animate_line_file.py <input_csv> <output_gif>")
        sys.exit(1)

    # get filenames from the user instead of hard-coding them
    input_file = sys.argv[1]
    output_file = sys.argv[2]

    # read the data produced by c program
    try:
        # use pandas to read the csv made in c
        df = pd.read_csv(input_file, comment='#', header=None)
        # column 1 is time, columns 2 onwards are the y-positions
        times = df.iloc[:, 1].values
        y_data_matrix = df.iloc[:, 2:].values
    except Exception as e:
        print(f"Error reading {input_file}: {e}")
        sys.exit(1)

    # set up the plot based on the number of points in the csv
    num_points = y_data_matrix.shape[1]
    x_positions = np.arange(num_points)
    y_positions = np.zeros(num_points)

    fig, ax1 = plt.subplots(1, 1)
    fig.suptitle(f'Simulation: {input_file}')
    ax1.set_ylim(-1.1, 1.1)
    ax1.set_xlim(0, num_points - 1)
    rope, = ax1.plot(x_positions, y_positions)

    # animation function
    def animate(frame):
        # pull the row of data for this specific time step
        current_y = y_data_matrix[frame]
        rope.set_ydata(current_y)
        return rope,

    # create the animation
    ani = animation.FuncAnimation(fig, animate, frames=len(times), blit=True)

    # save to the output name provided in the arguments
    print(f"Saving animation to {output_file}...")
    ani.save(output_file, writer='pillow', fps=25)
    print("Done!")


# we use this convention to ensure that if we import functions from this script, it is not executed
if __name__ == "__main__":
    main() # this is a good practice to get used to
