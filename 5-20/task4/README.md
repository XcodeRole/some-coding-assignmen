# Problem 4
Given a function header as char *findC (char const *source,char const *obj):
Write a program to achieve the following tasks:
1. Write a function to search a character sequence pointed to by a pointer (called "obj"),in another character sequence (called "source").Return the pointer pointing to the found character.If there is more than one target found in the source,return the pointer pointing to the first one.(2 marks)
    Egl:search for“C”in“ABCDEF”，return the pointer point to‘C'.
    Eg2:search for“Z”in“ABCDEF',return a NULL pointer..
    Eg3:search for“CD”in“ABCDEF',return the pointer point to‘C'.
    Eg4:search for "CF"in "ABCDEF",return a NULL pointer.
    Eg5:search for“A”in“ABCAFC',return the pointer point to the first‘A'.   
2. Write a main function to input two strings by the keyboard and print the position of the pointer in the source string(print-1 if the function returns a NULL pointer).(1 mark)
3. Compile and run correctly.(2 marks)