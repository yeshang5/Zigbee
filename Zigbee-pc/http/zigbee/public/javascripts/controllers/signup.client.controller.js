"use strict";

angular.module("webapp")
    .controller('signupController',['$scope','signupService',signupController]);

//常量
var MSG = {
    USER_EXIST:1,
    USER_SUCCESS:2,
    USER_AUTHFAIL:3    //非管理员账号权限不够
};


function signupController($scope,signupService){
    $scope.formDate = {};
    $scope.alertClass = "alert alert-info";
    $scope.msg = '请妥善保管设置的密码!';

    //清除提示
    $scope.clearAlert = function(){
        $scope.alertClass = "alert alert-info";
        $scope.msg = '请妥善保管设置的密码!';
    };


    //账号注册
    $scope.signupAccount = function(){
        signupService.signupAccount($scope.formDate).then(
            function(data){
                //$scope.msg = data;
                //alert(data.name);
                if(data.msg == MSG.USER_SUCCESS){
                    $scope.alertClass = "alert alert-success";
                    $scope.msg = '用户名'+$scope.formDate.name+'注册成功!';
                    $scope.formDate.name = '';
                    $scope.formDate.password = '';
                } else if(data.msg == MSG.USER_EXIST){
                    $scope.alertClass = "alert alert-danger";
                    $scope.msg = '用户名'+$scope.formDate.name+'已存在，注册失败!';
                    $scope.formDate.password = '';
                } else if(data.msg == MSG.USER_AUTHFAIL){
                    $scope.alertClass = "alert alert-danger";
                    $scope.msg = '当前登录用户不是管理员，没有注册权限，注册失败!';
                    $scope.formDate.password = '';
                    $scope.formDate.name = '';
                }
            },
            function(err){
                $scope.alertClass = "alert alert-danger";
                $scope.msg = '网络错误,注册失败!';
                $scope.formDate.password = '';
                $scope.formDate.name = '';
            }
        );
    };
}
