var express = require('express');

var moment = require('moment');
//console.log(moment().format('YYYY-MM-DD HH:mm:ss'));//获取当前时间
var path = require('path');

var ejs = require('ejs');   //模板引擎

var favicon = require('serve-favicon');
var logger = require('morgan');

var bodyParser = require('body-parser');    //把POST请求振文中的JSON数据解析为req.body属性

var cookieParser = require('cookie-parser');    //cookie 请求解析cookie，并将它们作为一个JS对象存储在req.cookies属性中
//var cookieSession = require('cookie-session');
var expressSession = require('express-session');    //session会话
//var mongoStore = require('connect-mongo')({session:expressSession}); //把mongoDB连接作为已通过分身验证的会话的持久性存储来注册

var crypto = require('crypto');   //加密算法


var mongoose = require('./config/mongoose.js');   //mongoose-mondodb数据库
var db = mongoose();

//var routes = require('./routes/index');
//var users = require('./routes/users');

var app = express();


// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.engine('.html',ejs.__express);
app.set('view engine', 'html');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
//app.use(bodyParser.json());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

app.use(cookieParser('MAGICString'));    //开启cookie
app.use(expressSession());
//app.use(cookieParser());
//app.use(cookieSession({secret:'MAGICALEXPRESSKEY'}));

//在req对象中添加了一个session属性，session对象是直接绑在MongoDB的sessions集合中
//当会话更改时，会话被保存在了数据库中
/*app.use(expressSession({
  secret:'SECRET',
  key:'zigbee',
  //cookie:{maxAge: 60*1000},    //session和相应的cookie生存时间1小时,如果不设置,默认为null,这样expire的时间就是浏览器的关闭时间

  store:  new mongoStore({        //创建新的mongodb数据库
    db:'zigbee',
    host:'localhost',
    port:27017,
    url:"mongodb://localhost/web",
    collection:'sessions'
  })
}));  //开启session,自动存入mongodb数据库，而非内存中*/

app.use(express.static(path.join(__dirname, 'public')));

//app.use('/', routes);
//app.use('/users', users);

/*路由算法*/

/*登录页和主页*/
require('./app/routes/login.server.route')(app);
require('./app/routes/home.server.route')(app);

/*基站管理*/
require('./app/routes/baseSignup.server.route')(app);


/*账号管理*/
require('./app/routes/userSignup.server.route')(app);
require('./app/routes/userAccount.server.route')(app);





// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});


module.exports = app;
