// Generierung von random Daten für Live Dashboard
// Author: Teodora Ljubevska

const fs = require("fs");

var logger = fs.createWriteStream("../Csv_data/live_data_demo.csv", {
  flags: "a", // 'a' means appending (old data will be preserved)
});

function writeNewLine() {
  const unixTimeNow = Math.floor(new Date().getTime() / 1000);

  //upper limit : 40, lower limit : 25
  const valueTemp = Math.random() * (32 - 22 + 1) + 22;

  //upper limit : 60, lower limit : 100 beats per minute
  const valueHeart = Math.random() * (100 - 60 + 1) + 60;

  //upper limit : 60, lower limit : 10
  const valueAcceleration = Math.random() * (60 - 10 + 1) + 10;

  // The floor() method rounds a number DOWNWARDS to the nearest integer
  logger.write("Temp," + valueTemp + "," + unixTimeNow + "\n");
}

//call writeNewLine every 1000 ms
setInterval(writeNewLine, 1000);

//logger.end();
