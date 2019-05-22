const express = require('express');
const bodyParser = require('body-parser');
const app = express();

const MongoClient = require('mongodb').MongoClient; 

app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
})); 

const port = 8092

const database = null;

MongoClient.connect( "mongodb://localhost:27017", function( err, db ) {
  if( err ) {
    console.log( 'Error: Cannot connect mondodb.', err );
  }
  else {
    console.log( 'Connected to mongo' );
    database = db;
  } } );

app.post('/', (req, res) => {
	//console.log( 'POST /' );
	console.log( 'POST /', req.body );
	// Watter Pump Seconds ';' Next Recodes in X minutes
	// wp: 3 minutes, nextRecordes in 4 Hours
	res.send( `^1;10^` );
	//res.send( `^${ 60 * 3 };${  60 * 4}^` );
} );

app.listen(port, () => console.log(`Example app listening on port ${port}!`))
