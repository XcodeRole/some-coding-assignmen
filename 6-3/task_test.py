

from task_functions import *

if __name__=='__main__':

    date="1/12/2022"
    format_date="January 12, 2022"
    assert format_date==get_written_date(date.split("/"))


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
    task={
            "name": "Finish checkpoint 1",
            "info": "Submit to Gradescope",
            "priority": 5,
            "duedate": '06/02/2022',
            "done": 'no'
        }
    assert is_valid_name(str2list[0])
    assert is_valid_priority(str2list[2], priority_scale)
    assert is_valid_date(str2list[3])
    assert is_valid_completion(str2list[4])
    assert get_new_task(str2list,priority_scale)==task


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

    assert is_valid_index("1",all_tasks,1)




