/*常量*/
module.exports = {
    //user.server.model
    USER:{
        ADMIN:'管理员',              //管理员
        OPER:'操作员'                //操作员
    },
    //signup.server.controller
    MSG:{
        USER_EXIST:1,       //账号已经存在
        USER_SUCCESS:2,     //账号操作成功
        USER_AUTHFAIL:3,    //非管理员账号权限不够
        USER_PSWERR:4,       //密码认证错误
        USER_DELFAIL:5      //删除失败
    }



};
