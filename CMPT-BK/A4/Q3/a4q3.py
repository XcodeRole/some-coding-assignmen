# CMPT 145 Course material
# Copyright (c) 2017-2021 Michael C Horsch
# All rights reserved.
#
# This document contains resources for homework assigned to students of
# CMPT 145 and shall not be distributed without permission.  Posting this
# file to a public or private website, or providing this file to a person
# not registered in CMPT 145, constitutes Academic Misconduct, according
# to the University of Saskatchewan Policy on Academic Misconduct.
#
# Synopsis:
#    Defines the GradeItem and the Student class
#
# A Grade Item is anything a course uses in a grading scheme,
# like a test or an assignment.
#
# A Student is a record of a student's identity, and includes the student's
# grade items.


class GradeItem(object):
    # A Grade Item is anything a course uses in a grading scheme,
    # like a test or an assignment.  It has a score, which is assessed by
    # an instructor, and a maximum value, set by the instructor, and a weight,
    # which defines how much the item counts towards a final grade.

    def __init__(self, weight, scored, out_of):
        """
        Purpose:
            Initialize the GradeItem object.
        Preconditions:
            :param weight: the weight of the GradeItem in a grading scheme
            :param scored: the scored obtained in the grade item
            :param out_of: the maximum possible score for the grade item
        """
        self.weight = weight
        self.scored = scored
        self.out_of = out_of

    def __str__(self):
        """
        Purpose:
            Represent the Grade object as a string.
            If the item has not been assessed yet, N/A is used.
            This function is used if we give a GradeObject to Python's str().
        Return:
            A string representation of the Grade.
        """
        if self.scored is None or self.out_of is None:
            return 'N/A'
        else:
            return str(self.scored) + '/' + str(self.out_of)

    def record_outcome(self, num, denom):
        """
        Purpose:
            Record the outcome of the GradeItem.
        Preconditions:
            :param n: The score obtained on the item
            :param d: The maximum score on the item
        Post-conditions:
            Changes the data stored in the GradeItem
        Return:  None
        """
        self.scored = num
        self.out_of = denom

    def contribution(self):
        """
        Purpose:
            Calculate and return the contribution this GradeItem makes
            to a grading scheme.  If the item has not been assessed,
            a value of zero is returned.
        Return:
            The weighted contribution of the GradeItem.
        """
        if self.scored is None or self.out_of is None:
            return 0
        else:
            return self.weight * self.scored / self.out_of


class StudentRecord(object):
    def __init__(self, first, last, st_number):
        """
        Purpose:  Initialize the StudentRecord object
        Preconditions:
            :param first: The student's first name, as a string
            :param last: The student's last name, as a string
            :param st_number: The student's id number, as a string
        """
        self.first_name = first
        self.last_name = last
        self.id = st_number
        self.midterm = None
        self.labs = []
        #add
        self.final_exam = None
        self.assignments=[]


    def calculate(self):
        """
        Purpose:
            Calculate the final grade in the course.
        Return:
            The final grade.
        """
        # modify
        return round(self.midterm.contribution()+self.final_exam.contribution()
                     + sum([b.contribution() for b in self.labs])
                     + sum([a.contribution() for a in self.assignments]) )

    def display(self):
        """
        Purpose:
            Display the information about the student, including
            all lab and assignment grades, and the calculation of the total grade.
        Return:
            None
        """
        print("Student:", self.first_name, self.last_name, 
              '(' + self.id + ')')
        print("\tCourse grade:", self.calculate())
        print('\tMidterm:', str(self.midterm))
        #

        print('\tLabs:', end=" ")
        for g in self.labs:
            if g.weight == 0:
                print('[' + str(g) + ']', end=' ')
            else:
                print(str(g), end=' ')
        #add
        print('\tFinal_exam:', str(self.final_exam))
        for g in self.assignments:
            if g.weight == 0:
                print('[' + str(g) + ']', end=' ')
            else:
                print(str(g), end=' ')
        print()


def read_student_record_file(filename):
    """
    Purpose:
        Read a student record file, containing information about the student.
        Line 1: Out of: the maximum score for the grade items
        Line 2: Weight: the weight of the grade items in the grading scheme
        Lines 3...: Student records as follows:
            Student Number, Lastname, Firstname, 10 lab marks, 10 assignment marks, midterm, final
    Pre-conditions:
        :param filename: A text file, comma-separated, with the above format.
    Return:
        :return: A list of StudentRecords constructed from the named file.
    """
    f = open(filename)

    # Second line tells us what the grade items are out of
    out_of_line = f.readline().rstrip().split(',')
    out_of = [int(i) for i in out_of_line[1:]]
    lab_out_of = out_of[0:10]
    assignments_out_of = out_of[10:20]
    mt_out_of = out_of[20]
    final_out_of = out_of[21]

     # Third line tells us the weight of each item
    weight_line = f.readline().rstrip().split(',')
    weights = [int(i) for i in weight_line[1:]]
    lab_weights = weights[0:10]
    assignment_weights = weights[10:20]
    mt_weight = weights[20]
    final_weight = weights[21]

    # Read all the students in the file:
    students = list()
    for line in f:
        student_line = line.rstrip().split(',')
        student = StudentRecord(student_line[2],student_line[1],student_line[0])
        labs = [int(lab) for lab in student_line[3:13]]
        assignments = [int(a) for a in student_line[13:23]]

        for g,o,w in zip(labs,lab_out_of,lab_weights):
            student.labs.append(GradeItem(w,g,o))
        for g,o,w in zip(assignments,assignments_out_of,assignment_weights):
            student.assignments.append(GradeItem(w,g,o))
        
        student.final_exam = GradeItem(final_weight,int(student_line[24]),final_out_of)

        student.midterm = GradeItem(mt_weight,int(student_line[23]),mt_out_of)

        students.append(student)

    return students

## MAIN PROGRAM STARTS ##

course = read_student_record_file('students.txt')

# display the students
for s in course:
    s.display()

## MAIN PROGRAM ENDS ##


