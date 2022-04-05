const { getAuth } = require('firebase-admin/auth');
const admin = require('firebase-admin');
const fs = require('fs');

const serviceAccount = require('./serviceAccountKey.json');
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL:
    'https://occupational-skills-training-default-rtdb.firebaseio.com',
});


const listAllUsers = (nextPageToken) => {
  // List batch of users, 1000 at a time.
  getAuth()
    .listUsers(1000, nextPageToken)
    .then((listUsersResult) => {
      listUsersResult.users.forEach((userRecord) => {
        users.push(userRecord['uid']);
        // console.log('user', userRecord.toJSON());
      });
      if (listUsersResult.pageToken) {
        // List next batch of users.
        listAllUsers(listUsersResult.pageToken);
      } else {
        getProfile();
      }
    })
    .catch((error) => {
      console.log('Error listing users:', error);
    });
};
// Start listing users from the beginning, 1000 at a time.
listAllUsers();

async function getProfile() {
  let i = 1;
  let row = 2;
  for (let uid of users) {
    console.log(i++);
    let result = await admin
      .database()
      .ref('/users/' + uid)
      .once('value');

    result = result.val();
    if (result && result.profile) {
      generateRow(uid, result.modify, result.profile, row++);
    }
    console.log(`row: ${row}`);
    // if (row > 90) {
    //   break;
    // }
  }
  wb.write('Excel.xlsx');
}

function generateRow(uid, timestamp, data, index) {
  ws.cell(index, 1).string(timestamp);
  ws.cell(index, 2).string(data.emailAddress);
  ws.cell(index, 3).string(data.givenName);
  ws.cell(index, 4).string(data.middleName);
  ws.cell(index, 5).string(data.lastName);
  ws.cell(index, 6).string(data.dateOfBirth);
  ws.cell(index, 7).string(data.gender);
  ws.cell(index, 8).string(data.homeAddress);
  ws.cell(index, 9).string(data.jobPosition);
  ws.cell(index, 10).string(data.department);
  ws.cell(index, 11).string(data.organization);
  ws.cell(index, 12).string(data.objective);
  ws.cell(index, 13).string(data.icdfTraining);
  ws.cell(index, 14).string(data.icdfProgramTitle);
  ws.cell(index, 15).string(data.icdfTrainingFrom);
  ws.cell(index, 16).string(data.icdfTrainingTo);
  ws.cell(index, 17).string(data.schoolName);
  ws.cell(index, 18).string(data.subject);
  ws.cell(index, 19).string(data.qualifications);
  ws.cell(index, 20).string(data.relatedJobExperience);
  ws.cell(index, 21).string(data.relatedJobDescription);
  ws.cell(index, 22).string(data.programme);
  ws.cell(index, 23).string(data.phoneNumber);
  ws.cell(index, 24).string(data.constituency);
  ws.cell(index, 26).string(data.recommendationName);

  if (data.nationIdCard) {
    if (/^data:application\/pdf/.test(data.nationIdCard)) {
      let base64 = data.nationIdCard.replace(
        /^data:application\/pdf;base64,/,
        ''
      );

      fs.writeFile(`assets/${uid}.pdf`, base64, 'base64', function (err) {
        console.log(err);
      });
      ws.cell(index, 25).link(`assets\\${uid}.pdf`);
    } else {
      let base64 = data.nationIdCard.replace(
        /^data:image\/[a-zA-Z]*;base64,/,
        ''
      );
      let imageType = data.nationIdCard.match(
        /^data:image\/([a-zA-Z]*);base64,/
      )[1];
      fs.writeFile(
        `assets/${uid}.${imageType}`,
        base64,
        'base64',
        function (err) {
          console.log(err);
        }
      );
      ws.cell(index, 25).link(`assets\\${uid}.${imageType}`);
    }
  }

  if (data.recommendationName) {
    if (/^data:application\/pdf/.test(data.recommendation)) {
      console.log(data.recommendation.slice(0, 500));
      const base64 = data.recommendation.replace(
        /^data:application\/pdf;base64,/,
        ''
      );

      fs.writeFile(
        `assets/${uid}-recommendation.pdf`,
        base64,
        'base64',
        function (err) {
          console.log(err);
        }
      );
      ws.cell(index, 27).link(`assets\\${uid}-recommendation.pdf`);
    } else {
      const base64 = data.recommendation.replace(
        /^data:image\/[a-zA-Z]*;base64,/,
        ''
      );
      const imageType = data.nationIdCard.match(
        /^data:image\/([a-zA-Z]*);base64,/
      )[1];
      fs.writeFile(
        `assets/${uid}-recommendation.${imageType}`,
        base64,
        'base64',
        function (err) {
          console.log(err);
        }
      );
      ws.cell(index, 27).link(`assets\\${uid}-recommendation.${imageType}`);
    }
  }
