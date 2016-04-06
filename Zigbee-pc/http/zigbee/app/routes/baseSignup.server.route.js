var baseSignupController = require('../controllers/baseSignup.server.controller');
var logoutController = require('../controllers/logout.server.controller');

module.exports = function(app){

    //账号注册
    app.route('/baseSignup')
        .get(baseSignupController.baseSignup)
        .post(baseSignupController.baseSignupAccount);

    app.get('/logout',logoutController.logout);       //注销登录
};
