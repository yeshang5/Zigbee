
var homeController = require('../controllers/home.server.controller');
var logoutController = require('../controllers/logout.server.controller');

module.exports = function(app){

    //系统主页
    app.route('/home')
        .get(homeController.home);                  //进入主页

    app.get('/logout',logoutController.logout);       //注销登录
};