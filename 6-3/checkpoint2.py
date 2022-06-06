def  save_temperature_statistics(input_dict, filename):
    import csv
    with open(filename, 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        for key,value in input_dict.items():
            #value is a list
            temperature_info_list =[key]+value
            csv_writer.writerow(temperature_info_list)

if __name__=='__main__':
    dict={
        "January":[67,47],
        "February":[45,89]
    }
    save_temperature_statistics(dict,"output.csv")