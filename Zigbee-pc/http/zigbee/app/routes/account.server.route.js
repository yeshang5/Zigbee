var accountController = require('../controllers/account.server.controller');
var logoutController = require('../controllers/logout.server.controller');

module.exports = function(app){

    //账号管理
    app.route('/account')
        .get(accountController.account);                  //进入用户列表页

    app.route('/account/accountTable')                    //显示列表
        .get(accountController.accountTable);

    app.route('/account/modifyPsw')                       //修改密码
        .post(accountController.modifyPsw);

    app.get('/logout',logoutController.logout);           //注销登录
};
