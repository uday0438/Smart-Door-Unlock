function doGet(e) {
  const ss = SpreadsheetApp.getActiveSpreadsheet();
  const sheet = ss.getSheetByName("Attendance");

  if (!sheet) {
    return ContentService
      .createTextOutput("Sheet 'Attendance' not found!")
      .setMimeType(ContentService.MimeType.TEXT);
  }

  // Read parameters
  const name = e.parameter.name || "Unknown";
  const hallTicket = e.parameter.hallTicket || "Unknown";
  const id = e.parameter.id || "0";
  const period = e.parameter.period || "0";
  const statusIN = e.parameter.statusIN || "NA";
  const statusOUT = e.parameter.statusOUT || "NA";

  // Determine FINAL STATUS
  let finalStatus = "Absent";
  if (statusIN === "Present" || statusOUT === "Present") {
    finalStatus = "Present";
  }
  if (statusIN === "Intruder" || statusOUT === "Intruder") {
    finalStatus = "Intruder";
  }

  const timestamp = new Date();

  // Append Row
  sheet.appendRow([
    timestamp,
    name,
    hallTicket,
    id,
    period,
    statusIN,
    statusOUT,
    finalStatus
  ]);

  return ContentService
    .createTextOutput("SUCCESS")
    .setMimeType(ContentService.MimeType.TEXT);
}

function doPost(e) {
  const data = JSON.parse(e.postData.contents);
  if (data.secret !== "Uday@9440100517") return ContentService.createTextOutput("Forbidden");

  const ss = SpreadsheetApp.getActiveSpreadsheet();
  const sheet = ss.getSheetByName("Attendance");
  
  // Save intruder image to Drive (Optional - for advanced users)
  // For now, we log the intruder entry
  sheet.appendRow([
    new Date(),
    data.name,
    "INTRUDER",
    "0",
    "0",
    "NA",
    "NA",
    "Intruder Captured"
  ]);

  return ContentService.createTextOutput("Intruder Logged");
}
