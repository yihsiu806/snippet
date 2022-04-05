const fs = require('fs');
const csv = require('@fast-csv/parse');
const bcrypt = require('bcrypt');

const mysql = require('mysql');
const connection = mysql.createConnection({
  host: 'localhost',
  user: 'ost',
  password: 'ost_2022',
  database: 'ost_new'
});
connection.connect((err) => {
  if (err) throw err;
  console.log('Connected!');
});

function onlyUnique(value, index, self) {
  return self.indexOf(value) === index;
}

let added = []

fs.createReadStream('ost.csv')
    .pipe(csv.parse())
    .on('error', error => console.error(error))
    .on('data', row => createUser(row))
    .on('end', rowCount => {
      console.log(`Parsed ${rowCount} rows`)
      let r = (Math.random() + 1).toString(36).substring(4);

      emails = emails.filter(onlyUnique);

      emails.forEach(async function (email) {
        
        const author = { email: email, password: r, 'created_at': new Date(), 'updated_at': new Date() };
        await new Promise((resolve, reject) => {
          connection.query('INSERT INTO users SET ?', author, (err, res) => {
            if(err) throw err;
            resolve();
            console.log('Last insert ID:', res.id);
          });
        })
      })
      
    })
    


let emails = [];


function createUser(row) {
  if (!row[1].includes('@')) {
    return;
  }
  emails.push(row[1])
  
  
}