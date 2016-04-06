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

    $scope.d_alertClass = "alert alert-info";
    $scope.d_msg = '注销账号需谨慎!';


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


    //账户注销
    $scope.deletePsw = function(){
        var loginType = $('#loginType').text();     //当前登录用户类型
        var deleteType = $('#d_userType').text();    //当前需删除用户类型
        $scope.formDate.name = $('#d_userName').text();
        $scope.formDate.adminName =  $('#loginName').text(); //当前登录用户名

        if(loginType == '管理员'){
            if(deleteType == '管理员'){
                $('#d_msg').removeClass();
                $('#d_msg').addClass('alert alert-danger');
                $('#d_msg_text').text('不能注销此账号,注销失败!');
            } else{
                accountService.deletePsw($scope.formDate).then(
                    function(data){
                        if(data.msg == MSG.USER_SUCCESS){
                            $('#d_msg').removeClass();
                            $('#d_msg').addClass('alert alert-success');
                            $('#d_msg_text').text('注销成功!');

                            //$table.bootstrapTable('remove',{field:})
                            var username = $.map($('#table').bootstrapTable('getSelections'), function (row) {
                                return row.username;
                            });

                            $('#table').bootstrapTable('remove', {
                                field: 'username',
                                values: username
                            });
                            //$('#delete').modal('hide');
                        } else if(data.msg == MSG.USER_PSWERR){
                            $('#d_msg').removeClass();
                            $('#d_msg').addClass('alert alert-danger');
                            $('#d_msg_text').text('管理员密码错误,注销失败!');
                        } else{
                            $('#d_msg').removeClass();
                            $('#d_msg').addClass('alert alert-success');
                            $('#d_msg_text').text('注销失败,刷新网页重新注销!');
                        }
                    },
                    function(err){

                    }
                );
            }
        } else {
            $('#d_msg').removeClass();
            $('#d_msg').addClass('alert alert-danger');
            $('#d_msg_text').text('操作员无权限,注销失败!');
        }
    }
}







//表格图标样式
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

//表格图标事件
window.accountEvents = {
    'click .edit': function (e, value, row, index) {
        $(document).ready(function(){
            $('#msg').removeClass();
            $('#msg').addClass('alert alert-info');
            $('#msg_text').text('请妥善修改密码!');

            $('#userName').val(row.username);   //jQuery 设置表单字段的值
            $('#userPassword').val('');
            $('#userNewPassword').val('');
            $('#modify').modal('show');
        });
    },
    'click .remove': function (e, value, row, index) {
        $(document).ready(function(){
            $('#d_msg').removeClass();
            $('#d_msg').addClass('alert alert-info');
            $('#d_msg_text').text('注销账号需谨慎!');


            $('#d_userPassword').val('');
            $('#d_userName').text(row.username);
            $('#d_userType').text(row.usertype);
            $('#d_createTime').text(row.createTime);
            $('#delete').modal('show');

        });

        //$('#table').bootstrapTable('removeAll');
    }
};

//表格条纹样式
function rowStyle(row, index) {
    if (index === 0 || index === 1) {
        return {
            classes: 'active'
        };
    }
    return {};
}

