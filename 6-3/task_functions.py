

def print_main_menu(menu):
    """
    :param menu: which to print
    :return: just print ,don't have return
    """
    print("==========================")
    print("What would you like to do?")
    for key,value in menu.items():
        print(str(key)+" - "+str(value))

def get_written_date(date_list):
    """
    :param date_list: you have to split date with "/"
    :return: return the format date
    """
    month_names = {
        1: "January",
        2: "February",
        3: "March",
        4: "April",
        5: "May",
        6: "June",
        7: "July",
        8: "August",
        9: "September",
        10: "October",
        11: "November",
        12: "December",
    }
    month_numeric = int(date_list[0])
    day = int(date_list[1])
    year = int(date_list[2])
    month = month_names[month_numeric]
    date = month + ' ' + str(day) + ', ' + str(year)
    return date   # Return the date string in written format


def get_selection(action, suboptions, to_upper=True, go_back=False):
    """
    param: action (string) - the action that the user
            would like to perform; printed as part of
            the function prompt
    param: suboptions (dictionary) - contains suboptions
            that are listed underneath the function prompt.
    param: to_upper (Boolean) - by default, set to True, so
            the user selection is converted to upper-case.
            If set to False, then the user input is used
            as-is.
    param: go_back (Boolean) - by default, set to False.
            If set to True, then allows the user to select the
            option M to return back to the main menu

    The function displays a submenu for the user to choose from.
    Asks the user to select an option using the input() function.
    Re-prints the submenu if an invalid option is given.
    Prints the confirmation of the selection by retrieving the
    description of the option from the suboptions dictionary.

    returns: the option selection (by default, an upper-case string).
            The selection be a valid key in the suboptions
            or a letter M, if go_back is True.
    """
    selection = None
    if go_back:
        if 'm' in suboptions or 'M' in suboptions:
            print("Invalid submenu, which contains M as a key.")
            return None
    while selection not in suboptions:
        print(f"::: What would you like to {action.lower()}?")
        for key in suboptions:
            print(f"{key} - {suboptions[key]}")
        if go_back == True:
            selection = input(f"::: Enter your selection "
                              f"or press 'm' to return to the main menu\n > ")
        else:
            selection = input("::: Enter your selection\n> ")
        if to_upper:
            selection = selection.upper()
        if go_back and selection.upper() == 'M':
            return 'M'
    print(f"You selected |{selection}| to",
          f"{action.lower()} |{suboptions[selection].lower()}|.")
    return selection


def print_task(task, priority_map, name_only=False):
    """
    param: task (dict) - a dictionary object that is expected
            to have the following string keys:
    - "name": a string with the task's name
    - "info": a string with the task's details/description
            (the field is not displayed if the value is empty)
    - "priority": an integer, representing the task's priority
        (defined by a dictionary priority_map)
    - "duedate": a valid date-string in the US date format:<MM>/<DD>/<YEAR>
            (displayed as a written-out date)
    - "done": a string representing whether a task is completed or not

    param: priority_map (dict) - a dictionary object that is expected
            to have the integer keys that correspond to the "priority"
            values stored in the task; the stored value is displayed for the
            priority field, instead of the numeric value.
    param: name_only (Boolean) - by default, set to False.
            If False, then only the name of the task is printed.
            Otherwise, displays the formatted task fields.
    returns: None; only prints the task values

    Helper functions:
    - get_written_date() to display the 'duedate' field
    """
    if name_only:
        print(task["name"])
    else:
        print(task["name"])
        info=task["info"]
        print(f"  * {info}")
        format_date=get_written_date(task["duedate"].split("/"))
        real_prio=priority_map[task["priority"]]
        print(f"  * Due: {format_date}  (Priority: {real_prio})")
        isdone=task["done"]
        print(f"  * Completed? {isdone}")





def print_tasks(task_list, priority_map, name_only = False,
                show_idx = False, start_idx = 0, completed = "all"):
    """
    param: task_list (list) - a list containing dictionaries
        with the task data
    param: priority_map (dict) - a dictionary object that is
        expected to have the integer keys that correspond to the
        "priority" values stored in the task; the stored value is
        displayed  for the priority field, instead of the numeric value.
    param: name_only (Boolean) - by default, set to False.
            If False, then only the name of the task is printed.
            Otherwise, displays the formatted task fields.
            Passed as an argument into the helper function.
    param: show_idx (Boolean) - by default, set to False.
            If False, then the index of the task is not displayed.
            Otherwise, displays the "{idx + start_idx}." before the
            task name.
    param: start_idx (int) - by default, set to 0;
            an expected starting value for idx that
            gets displayed for the first task, if show_idx is True.
    param: completed (str) - by default, set to "all".
            By default, prints all tasks, regardless of their
            completion status ("done" field status).
            Otherwise, it is set to one of the possible task's "done"
            field's values in order to display only the tasks with
            that completion status.

    returns: None; only prints the task values from the task_list

    Helper functions:
    - print_task() to print individual tasks
    """
    print("-"*42)
    idx=0
    for task in task_list: # go through all tasks in the list
        if idx>start_idx:
            if show_idx: # if the index of the task needs to be displayed
                print(f"{idx}.", end=" ")
            if completed == "all":
                print_task(task, priority_map, name_only)
            elif task["done"] == "yes":
                print_task(task, priority_map, name_only)
        idx+=1


def get_new_task(str2list,priority_map):
    """
    The function returns different types of values, depending on whether it succeeds or fails.
        The function expects the rst parameter to be a list with 5 strings.
        If the size of the list is not correct, then the function returns the integer size of
        the provided list. E.g., calling get_new_task() with an empty list as its rst
        argument should return 0 and so on.
        If any of the elements on the list are not of type string, the get_new_task()
        returns a tuple with ("type", <value>), where the <value> is substituted with
        the rst corresponding value from the list that was not a string.
        Each validation function will also be in charge of validating that its input
        parameter (the item from the list) is of the correct type (just in case it is called
        separately).
        If the size of the list is correct, the function calls the helper functions to validate the
        elds.
        If the validation succeeds, returns a new dictionary with the task keys set to
        the provided parameters (stripped of whitespace and converted to the proper
        type, if necessary).
        If any of the validation functions fail, returns a tuple with the name of the
        parameter and the corresponding value/parameter that caused it to fail
    """
    if len(str2list) != 5:
        return len(str2list)
    for elem in str2list:
        if type(elem) != str:
            return (type(elem),elem)
    if is_valid_name(str2list[0]) and is_valid_priority(str2list[2],priority_map)\
        and is_valid_date(str2list[3]) and is_valid_completion(str2list[4]):
        return {
            "name": str2list[0],
            "info": str2list[1],
            "priority": int(str2list[2]),
            "duedate": str2list[3],
            "done": str2list[4]
        }
    else:
        if not is_valid_name(str2list[0]):
            return ("name",str2list[0])
        if not is_valid_priority(str2list[2],priority_map):
            return ("priority",str2list[2])
        if not is_valid_date(str2list[3]):
            return ("duedate",str2list[3])
        if not is_valid_completion(str2list[4]):
            return ("done",str2list[4])

def is_valid_name(name):
    if type(name)==str and len(name) > 2 and len(name) < 25:
        return True
    return False

def is_valid_priority(priority,priority_map):
    if type(priority)!=str or not priority.isdigit() or int(priority) not in priority_map:
        return False
    return True


def is_valid_date(date):
    return type(date)==str and is_valid_year(date.split("/")) \
           and is_valid_month(date.split("/")) and is_valid_day(date.split("/"))

def is_valid_month(date_list):
    """
    Function
    """
    if len(date_list) == 3:
        for comp in date_list:
            if type(comp) is not str:  # if any one of the item is not a string, return False
                return False

        if date_list[0].isdigit():
            month = int(date_list[0])  # convert the string to integer
            if month >= 1 and month <= 12:
                return True
    return False  # invalid month


def is_valid_day(date_list):
    """
    The function ...
    """
    num_days = {
        1: 31,
        2: 28,
        3: 31,
        4: 30,
        5: 31,
        6: 30,
        7: 31,
        8: 31,
        9: 30,
        10: 31,
        11: 30,
        12: 31
    }
    if is_valid_month(date_list):
        if date_list[1].isdigit():
            month = int(date_list[0])
            day = int(date_list[1])
            if day >= 1 and day <= num_days[month]:
                return True  # valid day
    return False  # invalid day


def is_valid_year(date_list):
    if len(date_list) == 3:
        for comp in date_list:
            if type(comp) is not str:
                return False # current item is not a string, return False
        if date_list[2].isdigit():
            year = int(date_list[2])
            if year > 1000: # valid year
                return True
    return False

def is_valid_completion(completion):
    if type(completion) != str:
        return False
    if completion != 'yes' and completion !='no':
        return False
    return True


def is_valid_index(idx, in_list, start_idx=0):
    """
    param: idx (str) - a string that is expected to
            contain an integer index to validate
    param: in_list - a list that the idx indexes
    param: start_idx (int) - by default, set to 0;
            an expected starting value for idx that
            gets subtracted from idx for 0-based indexing

    The function checks if the input string contains
    only digits and verifies that (idx - start_idx) is >= 0,
    which allows to retrieve an element from in_list.

    returns:
    - True, if idx is a numeric index >= start_idx
    that can retrieve an element from in_list.
    - False if idx is not a string that represents an
    integer value, if int(idx) is < start_idx,
    or if it exceeds the size of in_list.
    """
    idx=str(idx)
    return idx.isdigit() and int(idx)>=start_idx and int(idx)>0 and int(idx) < len(in_list)


def update_task(info_list, idx, priority_map, field_key,
field_info, start_idx = 0):
    """
    param: info_list - a list that contains task dictionaries
    param: idx (str) - a string that is expected to contain an integer
            index of an item in the input list
    param: start_idx (int) - by default is set to 0;
            an expected starting value for idx that gets subtracted
                from idx for 0-based indexing
        param: priority_map (dict) - a dictionary that contains the
    mapping
                between the integer priority value (key) to its
    representation
                (e.g., key 1 might map to the priority value
    "Highest" or "Low")
                Needed if "field_key" is "priority" to validate its
    value.
        param: field_key (string) - a text expected to contain the
    name
                of a key in the info_list[idx] dictionary whose
    value needs to
                be updated with the value from field_info
        param: field_info (string) - a text expected to contain the
    value
                to validate and with which to update the dictionary
    field
                info_list[idx][field_key]. The string gets stripped
    of the
                whitespace and gets converted to the correct type,
    depending
                on the expected type of the field_key.

        The function first calls one of its helper functions
        to validate the idx and the provided field.
        If validation succeeds, the function proceeds with the
    update.

        return:
        If info_list is empty, return 0.
        If the idx is invalid, return -1.
        If the field_key is invalid, return -2.
        If validation passes, return the dictionary info_list[idx].
        Otherwise, return the field_key.

        Helper functions:
        The function calls the following helper functions:
        - is_valid_index()
        Depending on the field_key, it also calls:
        - is_valid_name()
        - is_valid_priority()
        - is_valid_date()
        - is_valid_completion()
        """
    idx=int(idx)
    task=info_list[idx]
    if len(info_list)==0:
        return 0
    if not is_valid_index(idx,info_list,start_idx):
        return -1
    if field_key not in task:
        return -2
    valid=False
    if field_key=="name":
        valid=is_valid_name(field_info)
    if field_key=="priority":
        valid=is_valid_priority(field_info,priority_map)
    if field_key=="duedate":
        valid=is_valid_date(field_info)
    if field_key=="done":
        valid=is_valid_completion(field_info)
    if valid:
        return task
    else:
        return field_key


def delete_item(in_list, idx, start_idx=0):
    """
    param: in_list - a list from which to remove an item
    param: idx (str) - a string that is expected to
            contain a representation of an integer index
            of an item in the provided list
    param: start_idx (int) - by default, set to 0;
            an expected starting value for idx that
            gets subtracted from idx for 0-based indexing

    The function first checks if the input list is empty.
    The function then calls is_valid_index() to verify
    that the provided index idx is a valid positive
    index that can access an element from info_list.
    On success, the function saves the item from info_list
    and returns it after it is deleted from in_list.

    returns:
    If the input list is empty, return 0.
    If idx is not of type string or start_idx is not an int,
return None.
    If is_valid_index() returns False, return -1.
    Otherwise, on success, the function returns the element
    that was just removed from the input list.

    Helper functions:
    - is_valid_index()
    """
    idx=str(idx)
    if len(in_list)==0:
        return 0
    if not idx.isdigit():
        return None
    if not is_valid_index(idx,in_list,start_idx):
        return -1
    ret=in_list[int(idx)]
    in_list.pop(int(idx))
    return ret


def load_tasks_from_csv(filename, in_list, priority_map):
    """
    param: filename (str) - A string variable which represents
the name of the file from which to read the contents.
    param: in_list (list) - A list of task dictionary objects
to which the tasks read from the provided filename are
appended. If in_list is not empty, the existing tasks are not
dropped.
    param: priority_map (dict) - a dictionary that contains the
mapping  between the integer priority value (key) to its
representation (e.g., key 1 might map to the priority value
"Highest" or "Low") Needed by the helper function.

    The function ensures that the last 4 characters of the
filename are '.csv'. The function requires the `import csv` and `import os`.

        If the file exists, the function will use the `with`
    statement to open the
        `filename` in read mode. For each row in the csv file, the
    function will
        proceed to create a new task using the `get_new_task()`
    function.
        - If the function `get_new_task()` returns a valid task
    object,it gets appended to the end of the `in_list`.
        - If the `get_new_task()` function returns an error, the 1-
    based row index gets recorded and added to the NEW list that is
    returned.
        E.g., if the file has a single row, and that row has
    invalid task data,
        the function would return [1] to indicate that the first
    row caused an error; in this case, the `in_list` would not be modified.
        If there is more than one invalid row, they get excluded
    from the in_list and their indices will be appended to the new list
    that's returned.

        returns:
        * -1, if the last 4 characters in `filename` are not '.csv'
        * None, if `filename` does not exist.
        * A new empty list, if the entire file is successfully read
    from `in_list`.
        * A list that records the 1-based index of invalid rows
    detected when calling get_new_task().

        Helper functions:
        - get_new_task()
        """
    import csv
    import os
    if not os.path.exists(filename):
        return None
    if os.path.splitext(filename)[-1]!="csv":
        return -1
    err=[]
    with open(filename,"r") as f:
        csv_reader=csv.reader(f)
        count=0
        for line in csv_reader:
            task=get_new_task(line,priority_map)
            if type(task)==dict:
                in_list.append(task)
            else:
                err.append(count+1)
            count+=1
    return err
            # task["name"]=line[0]
            # task["info"]=line[1]
            # task["priority"]=priority_map[line[2]]
            # task["duedate"]=line[3]
            # task["done"]=line[4]


def save_tasks_to_csv(tasks_list, filename):
    """
    param: tasks_list - The list of the tasks stored as
dictionaries
    param: filename (str) - A string that ends with '.csv' which
represents
               the name of the file to which to save the tasks.
This file will
               be created if it is not present, otherwise, it
will be overwritten.

    The function ensures that the last 4 characters of the
filename are '.csv'.
    The function requires the `import csv`.

    The function will use the `with` statement to open the file
`filename`.
    After creating a csv writer object, the function uses a `for`
loop
    to loop over every task in the list and creates a new list
    containing only strings - this list is saved into the file by
the csv writer
    object. The order of the elements in the list is:

    * `name` field of the task dictionary
    * `info` field of the task dictionary
* `priority` field of the task as a string
(i.e, "5" or "3", NOT "Lowest" or "Medium")
* `duedate` field of the task as written as string
(i.e, "06/06/2022", NOT "June 6, 2022")
* `done` field of the task dictionary

returns:
-1 if the last 4 characters in `filename` are not '.csv'
None if we are able to successfully write into `filename`
"""
    import os
    import csv
    if os.path.splitext(filename)[-1]!="csv":
        return -1
    with open(filename,"w") as f:
        csv_writer=csv.writer(f)
        for task in tasks_list:
            csv_writer.writerow([str(task["name"]),str(task["info"]),str(task["priority"]),str(task["duedate"]),str(task["done"])])

    return None