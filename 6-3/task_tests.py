

from task_functions import *

if __name__=='__main__':

    date="1/12/2022"
    format_date="January 12, 2022"
    assert get_written_date(date.split("/"))==format_date


    priority_scale = {
        1: "Lowest",
        2: "Low",
        3: "Medium",
        4: "High",
        5: "Highest"
    }
    str2list=["Finish checkpoint 1",
              "Submit to Gradescope",
              "5",
              '06/02/2022',
              'no'
              ]
    str2list1=["Finish checkpoint 1",
              "Submit to Gradescope",
              "5",
              '06/02/2022',
              0
              ]
    task={
            "name": "Finish checkpoint 1",
            "info": "Submit to Gradescope",
            "priority": 5,
            "duedate": '06/02/2022',
            "done": 'no'
        }
    assert is_valid_name(str2list[0])==True
    assert is_valid_priority(str2list[2], priority_scale)==True
    assert is_valid_date(str2list[3])==True
    assert is_valid_completion(str2list[4])==True
    assert get_new_task(str2list,priority_scale)==task

    del str2list[0]
    assert get_new_task(str2list,priority_scale)==4
    assert get_new_task(str2list1,priority_scale)==(int,0)

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

    del0_tasks=[
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
    assert is_valid_index("1",all_tasks,1)==True
    no_task=[]

    assert delete_item(all_tasks,0)==del0_tasks
    assert delete_item(no_task,0)==0
    assert delete_item(all_tasks,"a")==None
    assert delete_item(all_tasks,4)==-1






