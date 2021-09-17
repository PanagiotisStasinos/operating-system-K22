Panagiotis Stasinos
AM 1115201400191

Makefile : metaglwtizei ta verify.c root.c sort.c splitter.c searcher.c
		   me to make clean diagrafei ta .o ta uotput .bin kai tuxon .pipe

record.h : periexei to struct tou record kai struct gia lista apo records

searcher.c : 
// argv[0]  datafile
// argv[1]  height=1
// argv[2]  pattern
// argv[3]  skew_flag
// argv[4]  start
// argv[5]	end
// argv[6]  fifo
// argv[7]  fifo_
// argv[8]  root pid
dhmiourgei tria pipe , ena gia to plithos twn rec, ena ga ta recs kai ena gia ton xrono ekteleshs
otan teleiwsoun stelnoun signal sto root

splitter.c :
// argv[0]  datafile
// argv[1]  height
// argv[2]  pattern
// argv[3]  skew
// argv[4]  start
// argv[5]  end
// argv[6]  fifo
// argv[7]  fifo_
// argv[8]  root pid
ena pipe gia to plithos tvn rec, ena gia ta idia ta rec, kai ena gia tous xronous
ta diavazei apo ta pipes kai ta stelenei ston gonio

root.c :
kalei ton arxiko splitter kai sthn sunexeia afou lavei ta apotelesmata
dhmiourgei ena .bin me ta apotelesmata
kalei thn sort h opoia dhmioirgei to SortedResults.bin
telos ektupwnei ta statistika

sort.c :
taxinomei to ata3inomito arxeio kai to apothikeuei sto SortedResults.bin