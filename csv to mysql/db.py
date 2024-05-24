import mysql.connector
import csv

db = mysql.connector.connect(
  host = "",
  user = "",
  password = "",
  database = ""
)
cursor = db.cursor()

ids = set()
data = []

with open('sheet.csv') as file:
  file = csv.reader(file, delimiter=',')
  header = next(file)
  for row in file:

    if row[1] in ids:
      continue

    ids.add(row[2])
    value = (row[0], row[1], row[2], row[3], row[4], row[5], row[6])
    data.append(value)

query = """
  insert into `data`(`name`, `phone_number`, `address`,)
               values(%s, %s, %s)
"""
cursor.executemany(query, data)

db.commit()
cursor.close()
db.close()

    