import gzip


g = gzip.open("data_reviewed.gz", "w")
put_labels_in_first_line = False

with gzip.open("/home/esaule1/data/ebd_relAug-2016.txt.gz") as f:
  
  labels_text = f.readline()
  if put_labels_in_first_line:
    g.write("\t".join(labels_text.split("\t")[22:26]) + "\n")

  for line in f:
    splitted = line.strip().split("\t")
    reviewed = splitted[-1].strip() != "0"
    if reviewed:
      g.write("\t".join(splitted[22:26]) + "\n")

g.close()
