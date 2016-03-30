"use strict";

angular.module("webapp")
    .controller('CmdController',['$scope','CmdService',CmdController]);
   // .controller('CmdController',['$scope','CmdService',CmdController]); 可以进行链式调用

function CmdController($scope,CmdService){

    $scope.loadCmd = function(){
        CmdService.save().then(
            function(data){
                $scope.save = data;
            },
            function(err){

            }
        );
    };
    $scope.loadCmd();
}


