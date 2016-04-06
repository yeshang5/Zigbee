var signupController = require('../controllers/userSignup.server.controller');
var logoutController = require('../controllers/logout.server.controller');

module.exports = function(app){

    //账号注册
    app.route('/signup')
        .get(signupController.signup)                  //进入主页
        .post(signupController.signupAccout);

    app.get('/logout',logoutController.logout);       //注销登录
};