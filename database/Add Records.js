_plant_name_ = "Marie";

d = new Date();

function createRecord( _plant_name_ ) {
    d = new Date( d - ( 1000 * 60 * 60 * 2 ) );
    return {
        "plant" : _plant_name_,
        "volt" : "12." + Math.floor(Math.random() *24) + 1,
        "batt" : Math.floor(Math.random() * 80) + 1,
        "moisture" : 345 + Math.floor(Math.random() * 232) + 1,
        "temperature" : ( Math.floor(Math.random() * 120) + 100 ) / 10.0 ,
        "dateTime" : d
    };
};

function populate( plantName ) {
    d = new Date();
    let data = [];
    nb = 120;
    for ( let i = 0; i < nb; i++ ) {
        data.push( createRecord( plantName ) );
    }
    db.getCollection('records').deleteMany( { plant: plantName } );
    db.getCollection('records').insertMany( data );
}

populate( 'Marie' );
populate( 'Ficus' );
populate( 'Geranium' );
populate( 'Jasmin' );
//db.getCollection('records').find( {} )
