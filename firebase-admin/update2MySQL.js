const xlsx = require('node-xlsx');
const axios = require('axios');

const workSheetsFromFile = xlsx.parse(`${__dirname}/candidates.xlsx`);

const forLoop = async (_) => {
  for (let i = 1; i < workSheetsFromFile[0].data.length; i++) {
    console.log(i);
    let row = workSheetsFromFile[0].data[i];

    // for (let j = 0; j < row.length; j++) {
    //   row[j] = row[j].replace('"', '\\"');
    //   console.log(row[j])
    // }
    row = row.map((str) => {
      return str.replace(/"/g, '\\"');
    });

    await axios
      .post('http://localhost/ost/api/api.php?query=profile', {
        timestamp: row[0] || '',
        emailAddress: row[1] || '',
        givenName: row[2] || '',
        middleName: row[3] || '',
        lastName: row[4] || '',
        dateOfBirth: row[5] || '',
        gender: row[6] || '',
        homeAddress: row[7] || '',
        jobPosition: row[8] || '',
        organization: row[9] || '',
        department: row[10] || '',
        objective: row[11] || '',
        icdfTraining: row[12] || '',
        icdfTrainingTitle: row[13] || '',
        icdfTrainingFrom: row[14] || '',
        icdfTrainingTo: row[15] || '',
        schoolName: row[16] || '',
        subject: row[17] || '',
        qualifications: row[18] || '',
        relatedJobExperience: row[19] || '',
        relatedJobDescription: row[20] || '',
        programme: row[21] || '',
        phoneNumber: row[22] || '',
        constituency: row[23] || '',
        nationalIDCard: row[24] || '',
        recommendation: row[26] || '',
      })
      .then(function (response) {
        console.log(response.data);
      })
      .catch(function (error) {
        console.log(error);
      });
  }
};

forLoop();
