const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const moment = require( 'moment' );
const app = express();

// API
app.use( cors( { origin: 'http://192.168.0.17' } ) );
app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
	extended: true
})); 
const port = 8092;
//

// Database
const databaseURL = 'mongodb://127.0.0.1:27017';
const databaseName = 'ardugrow';
const mongoClient = require('mongodb').MongoClient; 
let plants;

if ( !plants ) {
	mongoClient.connect( databaseURL, ( err, db ) => {
		if ( err ) {
			console.log( 'ERROR: Cannot connect mongodb', err );
			data.error = err;
			res.send( JSON.stringify( data, null, 2 ) );
		}
		else {
			const ardugrowDb = db.db( databaseName );
			const cursor = ardugrowDb.collection('plants').find().toArray( ( err, result ) => {
				if ( err ) {
					console.log( 'Cannot fetch plants', err );
				} else {
				// 	result.forEach( item => {
				// 		item.date = moment( item.dateTime ).fromNow();
				// 		item.calendar = moment( item.dateTime).calendar();
				// 	} );
				 	plants = result;
				 	console.log( "plants: ", plants );
				}
				db.close();
			} );;

		} } );
}
//


app.post('/', (req, res) => {
	console.log( new Date().toISOString() + ' POST /', req.body );
	let data = req.body;
	data.dateTime = new Date();

	mongoClient.connect( databaseURL, ( err, db ) => {
		if ( err ) {
			console.log( 'ERROR: Cannot connect mongodb', err );
		}
		else {
			const ardugrowDb = db.db( databaseName );
			ardugrowDb.collection( 'records' ).insertOne( data, ( err, res ) => {
				if ( err ) {
					console.log( 'ERROR: Cannot save data to mongodb collection.', err );
				}
				db.close();
			} );
		} } );

	// Watter Pump Seconds ';' Next Recodes in X minutes
	// wp: 3 minutes, nextRecordes in 4 Hours
	res.send( `^0;90^` );
} );

app.get( '/', ( req, res ) => {
	res.setHeader('Access-Control-Allow-Origin', '*');
	res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');
	let data = {};
	mongoClient.connect( databaseURL, ( err, db ) => {
		if ( err ) {
			console.log( 'ERROR: Cannot connect mongodb', err );
			data.error = err;
			res.send( JSON.stringify( data, null, 2 ) );
		}
		else {
			const ardugrowDb = db.db( databaseName );
			let p = [];
			let data = [];
			plants.forEach( plant => {
				p.push( new Promise( ( resolve, reject) => { 
					ardugrowDb.collection('records') .find( { p: 'Ficus' } ).sort( { dateTime: -1 } ).limit( 150 ).toArray( ( err, result ) => {
						if ( err ) {
							data.error = err;
							throw( err );
						} else {

							if ( result.length === 0 ) 
								throw( 'No records', plant );

							result.forEach( item => {
								delete( item.p );
								delete( item._id );
								item.timeFormat = moment( item.dateTime ).format( 'MM/DD/YYYY' );
								item.m = 1024 - item.m;
							} );
							result = result.reverse();
							resolve( {
								name: plant.name,
								moment: moment( result[ result.length - 1 ].dateTime ).calendar(),
								battery: result[ result.length - 1 ].b,
								voltage: result[ result.length - 1 ].v,
								moisture: result[ result.length - 1 ].m,
								temperature: result[ result.length - 1 ].t,
								trigger: plant.trigger,
								records: result
							} );
						} 
				} ) } ) ) }
			);
			Promise.all( p ).then( values => {
				db.close();
				res.send( JSON.stringify( values, null, 2 ) );
			} );
		}
	} );
} );

app.listen(port, () => console.log(`Example app listening on port ${port}!`));
