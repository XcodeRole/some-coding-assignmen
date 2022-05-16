# Problem 6
Given a set of x,y data points,it is often necessary to automatically calculate an equation which gives the best fit line through the data.This type of analysis is known as line fitting or regression analysis.
The mathematical basis behind line fitting is shown below.You have a set of data points represented by (x1.y1),(x2.y2).....(xn,yn).You want an equation which represents this data,the exact type of equation will depend on the way in which the data varies.Given any mathematical relationship between x and y it should be possible to generate a line fitting algorithm.The line fitting algorithm is described below.
## Straight line
The equation for a straight line is:y=ax +b.Suppose that we have n points of(x,y)values from the text file,the values of a and b for the best fit line that is the best approximation of the given set of data points can be obtained from the following equation:

....omit....

# run it
command : make && ./task6 < pre_input.txt
# Notes
pre_input.txt is the debug case file used to replace (or simulate) standard input
If you want to input manually, please remove "< pre_input.txt" from the command line and replace it with keyboard input follow the prompts output.

# pre_input.txt
first line is the number of points you want enter
second to twelfth is those points you will enter next
thirteenth is the point you want delete from the file