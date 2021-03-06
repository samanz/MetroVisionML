#!/usr/bin/python
import _mysql, os, zlib, MySQLdb
from PIL import Image

dbname = "metrovision"

class metrovisionDB:
  def __init__(self, server, user, password, ext, width, height, dataset):
    self.format_id = 0
    self.dataset_id = 0
    self.dataset = dataset
    self.server = server
    self.user = user
    self.password = password
    self.ext = ext
    self.width = width
    self.height = height
    self.db = _mysql.connect(server, user, password, dbname) 
    self.cwd = os.getcwd() # get full path of current directory
    self.storeDatasetId()
    self.storeFormatId()
    self.readDir() 
 
  def readDir(self):
    for drc in os.listdir(self.cwd):
       if(raw_input("store " + drc + " ?").upper() =='Y'):
         self.tagfiles(os.path.join(self.cwd, drc), drc) # os.path.join for os indy

  def tagfiles(self, drc, tag):
    for subdir, drs, files in os.walk(drc):
      for file in files:
        if(file.find(self.ext) != -1):
          print file
          f = Image.open(os.path.join(drc, file), 'r')
          image = self.db.escape_string(f.tostring())
	  print len(f.tostring())
          self.storeImage(f.tostring(), tag)
          
  def storeImage(self, image, tag):
    class_id = self.getClassificationId(tag)
    query = 'INSERT INTO item (width, height, image, classification_id, dataset_id, format_id) VALUES ("'
    query += self.width + '","'
    query += self.height + '","'
    query += '","'
    #query += image + '","'
    query += class_id + '","'
    query += self.dataset_id + '","'
    query += self.format_id + '"'
    query += ')'
    self.db.query(query)
    id_query = 'SELECT id from item where dataset_id = ' + self.dataset_id + ' ORDER BY id DESC'
    self.db.query(id_query)
    r=self.db.store_result()
    insert_id = r.fetch_row()[0][0]
    print insert_id
    query2 = "UPDATE item SET image = CONCAT(image,"
    query2 += '"' + self.db.escape_string(image) + '"'
    query2 += ") WHERE id = "
    query2 += insert_id
    self.db.query(query2) 
    print "Successfuly tagged image as", tag 

  def storeFormatId(self):
    theExt = 'RAW'
    query = 'SELECT COUNT(*) FROM format WHERE type = "' + theExt + '"'
    self.db.query(query)
    r = self.db.store_result()
    value = r.fetch_row()[0][0]
    if not int(value):
      query = 'INSERT INTO format (type) VALUES("' + theExt + '")'
      self.db.query(query)
    query = 'SELECT id FROM format WHERE type = "' + theExt + '"'
    #print query
    self.db.query(query)
    r = self.db.store_result()
    self.format_id = r.fetch_row()[0][0]

  def getClassificationId(self, name):
    self.db.query('SELECT COUNT(*) FROM classification WHERE name = "' + name + '"')
    r = self.db.store_result()
    value = r.fetch_row()[0][0]
    if not int(value):
      print "No classification by name ", name, " creating new classification"
      u_low = raw_input("What low value of the u range (0 for idk)? ")
      u_high = raw_input("What high value of the u range (255 for idk)? ")
      v_low = raw_input("What low value of the v range (0 for idk)? ")
      v_high = raw_input("What high value of the v range (255 for idk)? ")
      query = 'INSERT INTO classification (name, u_low, u_high, v_low, v_high) VALUES ('
      query += '"' + name + '","' + u_low + '","' + u_high + '","'+ v_low + '","' + v_high + '")'
      self.db.query(query)
      print "Classification creation successful"
    query = 'SELECT id FROM classification WHERE name = "' + name + '"'
    self.db.query(query)
    r = self.db.store_result()
    return r.fetch_row()[0][0]

  def storeDatasetId(self):
    self.db.query('set global max_allowed_packet=1000000000')
    query = 'SELECT COUNT(*) FROM dataset WHERE name = "' + dataset + '"'
    self.db.query(query)
    r = self.db.store_result()
    value = r.fetch_row()[0][0]
    if not int(value):
      print "No dataset by name ", dataset, " creating new dataset"
      source = raw_input("What is the source of this data? ")
      description = raw_input("Please insert short description of data: ")
      query = 'INSERT INTO dataset (name, source, description) VALUES ('
      query += '"' + dataset + '","' + source + '","' + description + '")'
      #print query
      self.db.query(query)
      print "Dataset creation successful"
    query = 'SELECT id FROM dataset WHERE name = "' + dataset + '"'
    self.db.query(query)
    r = self.db.store_result()
    self.dataset_id = r.fetch_row()[0][0]

dataset = raw_input("Dataset name: ")

server = raw_input("Database Server: ")

user = raw_input("Username: ")

password = raw_input("Password: ")

fileext = raw_input("Extension of image files: ")

width = raw_input("Image file width: ")

height = raw_input("Image file height: ")

metro = metrovisionDB(server, user, password, fileext, width, height, dataset)
