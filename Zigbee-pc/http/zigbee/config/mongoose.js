var mongoose = require('mongoose');
var config = require('./config');


module.exports = function(){
    var db = mongoose.connect(config.mongodb,function(err){
        if(err){
            console.log(err);
        }

        console.log('Connect to mongoDB success! ');
    });

    //console.log(mongoose.connection.db);

    require('../app/models/user.server.model.js');

    return db;
};