const express = require('express');
const bodyParser = require('body-parser');
const app = express();

app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
})); 

const port = 8092

app.get('/', (req, res) => {
	console.log( 'GET /', req );
	//console.log( 'GET /' );
	res.send('Hello World!');
} );

app.post('/', (req, res) => {
	//console.log( 'POST /' );
	console.log( 'POST /', req.body );
	res.send('Hello World!!');
} );

app.listen(port, () => console.log(`Example app listening on port ${port}!`))
