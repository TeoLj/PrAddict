// Generierung von random Daten für Live Dashboard
// Author: Teodora Ljubevska

const fs = require("fs");

var logger = fs.createWriteStream("../CSV_data/live_data_demo.csv", {
  flags: "a", // 'a' means appending (old data will be preserved)
});

logger.write("measurement," + "values," + "unix_timestamp" + "\n");

function writeNewLine() {
  const unixTimeNow = Math.round(Math.floor(new Date().getTime() / 1000));

  //upper limit : 40, lower limit : 25
  const valueTemp = Math.round(Math.random() * (40 - 25 + 1) + 25);

  //upper limit : 60, lower limit : 100 beats per minute
  const valueHeart = Math.round(Math.random() * (100 - 60 + 1) + 60);

  //upper limit : 60, lower limit : 10
  const valueAcceleration = Math.round(Math.random() * (60 - 10 + 1) + 10);

  const valHum = Math.round(Math.random() * (100 - 80 + 1) + 80);

  // The floor() method rounds a number DOWNWARDS to the nearest integer
  logger.write("Temp," + valueTemp + "," + unixTimeNow + "\n");
  logger.write("Heart," + valueHeart + "," + unixTimeNow + "\n");
  logger.write("Acceleration," + valueAcceleration + "," + unixTimeNow + "\n");
  logger.write("Hum," + valHum + "," + unixTimeNow + "\n");
}

//call writeNewLine every 10000 ms (10s)
setInterval(writeNewLine, 10000);

//logger.end();
