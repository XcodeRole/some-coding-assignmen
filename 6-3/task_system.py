
from task_functions import *
if __name__=='__main__':
    the_menu={
        "L": "List",
        "A": "Add",
        "U": "Update",
        "D": "Delete",
        "S": "Save the data to file",
        "R": "Restore data from file",
        "Q": "Quit this program"
    }

    all_tasks = [
        {
            "name": "Call XYZ",
            "info": "",
            "priority": 3,
            "duedate": '05/28/2022',
            "done": 'yes'
        },
        {
            "name": "Finish checkpoint 1 for CSW8",
            "info": "Submit to Gradescope",
            "priority": 5,
            "duedate": '06/02/2022',
            "done": 'no'
        },
        {
            "name": "Finish checkpoint 2 for CSW8",
            "info": "Implement the new functions",
            "priority": 5,
            "duedate": '06/05/2022',
            "done": 'no'
        }

    ]

    list_menu = {
        "A": "all tasks",
        "C": "completed tasks",
        "I": "incomplete tasks"
    }

    priority_scale = {
        1: "Lowest",
        2: "Low",
        3: "Medium",
        4: "High",
        5: "Highest"
    }

    opt = None
    while True:
        print_main_menu(the_menu)
        opt = input("::: Enter a menu option\n> ")
        opt = opt.upper()
        if opt not in the_menu:
            print(f"WARNING: {opt} is an invalid menu option.\n")
            continue
        print(f"You selected option {opt} to > {the_menu[opt]}.")

        if opt=="Q":
            print("Goodbye!\n")
            break  # exit the main `while` loop

        elif opt == 'L':
            if all_tasks == []:
                print("WARNING: There is nothing to display!")
                # Pause before going back to the main menu
                input("::: Press Enter to continue")
                continue
            subopt = get_selection(the_menu[opt], list_menu)
            if subopt == 'A':
                print_tasks(all_tasks, priority_scale)
            elif subopt == 'C':
                print_tasks(all_tasks, priority_scale, completed='yes')
            elif subopt == 'I':
                print_tasks(all_tasks, priority_scale, completed='no')

        elif opt == 'A':
            continue_action = 'y'
            while continue_action == 'y':
                print("::: Enter each required field, separated by commas.")
                print("::: name, info, priority, MM/DD/YYYY, is task done? (yes/no)")
                raw_str = input("> ")  # TODO: get and process the data into a list
                str2list = raw_str.split()
                result = get_new_task(str2list,priority_scale)  # TODO: attempt to create a new task
                if type(result) == dict:
                    all_tasks.append(result)  # TODO: add a new task to the list of tasks
                    print(f"Successfully added a new task!")
                    print_task(result, priority_scale)
                elif type(result) == int:
                    print(f"WARNING: invalid number of fields!")
                    print(f"You provided {result}, instead of the expected 5.\n")
                else:
                    print(f"WARNING: invalid task field: {result}\n ")

                print("::: Would you like to add another task?", end=" ")
                continue_action = input("Enter 'y' to continue.\n> ")
                continue_action = continue_action.lower()

        elif opt == 'U':
            continue_action = 'y'
            while continue_action == 'y':
                if all_tasks == []:  # TODO
                    print("WARNING: There is nothing to update!")
                    break
                print("::: Which task would you like to update?")
                print_tasks(all_tasks, priority_scale, name_only=True, show_idx=True, start_idx=1)
                print("::: Enter the number corresponding to the task. ")
                user_option = input("> ")
                if is_valid_index(user_option,all_tasks,start_idx=1):  # TODO
                    idx=int(user_option)-1  # TODO: convert the index appropriately to account for the start_idx = 1
                    subopt = get_selection("update", all_tasks[idx], to_upper = False, go_back = True)
                    if subopt == 'M':  # if the user changed their mind
                        break
                    print(f"::: Enter a new value for the field | {subopt} | ") # TODO
                    field_info = input("> ")
                    result = update_task(all_tasks, user_option, priority_scale, subopt, field_info, start_idx=1)
                    if type(result) == dict:
                        print(f"Successfully updated the field | {subopt} |: ")  # TODO
                        print_task(result, priority_scale)  # TODO
                    else:  # update_task() returned an error
                        print(f"WARNING: invalid information for the field | {subopt} |!")  # TODO
                        print(f"The task was not updated.")
                else:  # is_valid_index() returned False
                    print(f"WARNING: |{...}| is an invalid task  number!")  # TODO
                print("::: Would you like to update another task?",end=" ")
                continue_action = input("Enter 'y' to continue.\n>")
                continue_action = continue_action.lower()


        elif opt=="D":
            continue_action = 'y'
            while continue_action == 'y':
                print("You selected option D to > Delete.")
                print("Which task would you like to delete?")
                print("A - Delete all tasks at once")
                print("------------------------------------------")
                count=0
                for task in all_tasks:
                    print(str(count)+". "+str(all_tasks[count]["name"]))
                    count+=1
                print("::: Enter the number corresponding to the task")
                print("::: or press 'M' to return to the main menu. ")
                idx = input(">")
                if idx=="M":
                    break
                elif idx.isdigit():
                    status=delete_item(all_tasks,idx,start_idx=0)
                    if type(status)==dict:
                        del_item=status["name"]
                        print("Success!")
                        print(f"Deleted the task|{del_item}|")
                    else:
                        print("WARNING: There is nothing to delete!")
                elif idx=="A":
                    print("::: WARNING! Are you sure you want to delete All tasks? ")
                    print("::: Type Yes to continue the deletion.")
                    yes=input(">")
                    if yes=="yes":
                        print("Deleted all tasks.")
                        all_tasks.clear()
                else:
                    print(f"WARNING: |{idx}| is an invalid task number!")
                print("::: Would you like to delete another task? Enter 'y' to continue.")
                continue_action=input(">")

        elif opt == "R":
            continue_action = 'y'
            while continue_action == 'y':
                print("You selected option R to > Restore data from file.")
                print("::: Enter the filename ending with '.csv'. ")
                filename=input(">")
                err=load_tasks_from_csv(filename,all_tasks,priority_scale)
                if err==None:
                    print(f"WARNING: |{filename}| was not found!")
                if len(err)!=0:
                    print(f"WARNING: |{filename}| contains invalid data!")
                    print("The following rows from the file were not loaded:")
                    print(err)
                else:
                    print(f"Successfully stored all tasks to |{filename}|")
                    break
                print("::: Would you like to load another file? Enter 'y' to try again.")
                continue_action=input(">")

        elif opt == 'S':
            continue_action = 'y'
            while continue_action == 'y':
                print("::: Enter the filename ending with '.csv'.")
                filename = input("> ")
                err = save_tasks_to_csv(all_tasks,filename)  # TODO: Call the function with appropriate inputs and capture the output

                if err == -1:  # TODO
                    print(f"WARNING: |{filename}| is an invalid file name!") # TODO
                    print("::: Would you like to try again?", end="")
                    continue_action = input("::: Enter 'y' to try again.\n > ")

                else:
                    print(f"Successfully stored all the tasks to | {filename} | ")

        input("::: Press Enter to continue")
        # end of the program
    print("Have a nice day!")