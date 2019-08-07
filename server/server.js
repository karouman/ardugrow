const express = require('express');
const bodyParser = require('body-parser');
const app = express();

const mongoClient = require('mongodb').MongoClient; 

app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
})); 

const port = 8092;

app.post('/', (req, res) => {
	console.log( new Date().toISOString() + ' POST /', req.body );
	let data = req.body;
	data.dateTime = new Date();

	mongoClient.connect( 'mongodb://127.0.0.1:27017', ( err, db ) => {
	  if ( err ) {
	    console.log( 'ERROR: Cannot connect mongodb', err );
	  }
	  else {
	    const ardugrowDb = db.db( 'ardugrow' );
	    ardugrowDb.collection( "test" ).insertOne( data, ( err, res ) => {
	      if ( err ) {
		console.log( 'ERROR: Cannot save data to mongodb collection.', err );
	      }
	      db.close();
	    } );
	  } } );

	// Watter Pump Seconds ';' Next Recodes in X minutes
	// wp: 3 minutes, nextRecordes in 4 Hours

	  //console.log( 'Reply: 2;1' );

	res.send( `^20;1^` );
} );

app.listen(port, () => console.log(`Example app listening on port ${port}!`));
