"use strict";

angular.module("webapp")
    .controller('accountController',['$scope','accountService',accountController]);

//常量
var MSG = {

    USER_SUCCESS:2,
    USER_AUTHFAIL:3,
    USER_PSWERR:4       //密码认证错误
};



function accountController($scope,accountService){
    $scope.formDate = {};
    $scope.alertClass = "alert alert-info";
    $scope.msg = '请妥善修改密码!';


    //清除提示
    $scope.clearAlert = function(){
        $scope.alertClass = "alert alert-info";
        $scope.msg = '请妥善修改密码!';
    };

    //修改密码
    $scope.modifyPsw = function(){
        var loginName = $('#loginName').text();
        $scope.formDate.name  = $('#userName').val();

        if(loginName == $scope.formDate.name){  //登录用户和需修改用户一致
            accountService.modifyPsw($scope.formDate).then(
                function(data){
                    if(data.msg == MSG.USER_SUCCESS){
                        $scope.alertClass = "alert alert-success";
                        $scope.msg = '修改成功!';
                    } else if(data.msg == MSG.USER_PSWERR){
                        $scope.alertClass = "alert alert-danger";
                        $scope.msg = '密码错误,修改失败!';
                    } else if(data.msg == MSG.USER_AUTHFAIL){
                        $scope.alertClass = "alert alert-danger";
                        $scope.msg = '请不要修改他人账号!';
                    }
                    $scope.formDate.password = '';
                    $scope.formDate.newPassword = '';
                },
                function(err){
                    $scope.alertClass = "alert alert-danger";
                    $scope.msg = '网络错误,修改失败!';
                    $scope.formDate.password = '';
                    $scope.formDate.newPassword = '';
                }

            );
        } else {
            $scope.alertClass = "alert alert-danger";
            $scope.msg = '请不要修改他人账号!';
            $scope.formDate.password = '';
            $scope.formDate.newPassword = '';
        }

    };
}







/*<!----------3.1 账号修改（模态框）--------->*/
//图标样式
function accountFormatter(value, row, index) {
    return [
        '<a class="edit ml10" href="javascript:void(0)" title="Edit">',
        '<i class="fa fa-edit fa-fw fa-lg"></i>',
        '</a>',
        '<a class="remove ml10" href="javascript:void(0)" title="Remove">',
        '<i class="fa fa-remove fa-fw fa-lg"></i>',
        '</a>'
    ].join('');
}

//图标事件
window.accountEvents = {
    'click .edit': function (e, value, row, index) {
        $(document).ready(function(){
            $('#userName').val(row.username);   //jQuery 设置表单字段的值
            $('#userPassword').val('');
            $('#userNewPassword').val('');
            $('#myModal').modal('show');
        });
    },
    'click .remove': function (e, value, row, index) {

    }
};
