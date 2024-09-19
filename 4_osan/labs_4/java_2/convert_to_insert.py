cnt = 0
query = []

with open('table.txt', 'r') as fl:
    lines = fl.readlines()
    current_data = []
    for line in lines:
        if cnt % 6 == 0 or cnt % 6 == 4:
            current_data.append(int(line))
            cnt += 1
            continue
        if cnt % 6 == 5:
            current_data.append(int(line))
            query.append(current_data)
            current_data = []
            cnt += 1
            continue

        current_data.append(line[:-1])
        cnt += 1

for insert in query:
    # print(insert)
    print('INSERT into people (id, lastname, firstname, secondname, age, oid)\nVALUES (' + str(insert)[1:-1] + ');')