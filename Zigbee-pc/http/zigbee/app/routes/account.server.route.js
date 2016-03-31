var accountController = require('../controllers/account.server.controller');
var logoutController = require('../controllers/logout.server.controller');

module.exports = function(app){

    //首页
    app.route('/account')
        .get(accountController.account);                  //进入用户列表页

    app.route('/account/accountTable')
        .get(accountController.accountTable);

    app.get('/logout',logoutController.logout);           //注销登录
};
