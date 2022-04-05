const { getAuth } = require('firebase-admin/auth');
const admin = require('firebase-admin');

const serviceAccount = require('./serviceAccountKey.json');
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL:
    'https://occupational-skills-training-default-rtdb.firebaseio.com',
});

const fs = require('fs');

let users = [];

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

fs.appendFileSync('profiles.json', '[');

async function getProfile() {
  let i = 1;
  for (let uid of users) {
    console.log(++i);
    const result = await admin
      .database()
      .ref('/users/' + uid)
      .once('value');

    fs.appendFileSync('profiles.json', JSON.stringify(result.val()) + ',\n');
  }

  fs.appendFileSync('profiles.json', ']');
}
