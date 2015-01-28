(function() {
  'use strict';

  /**
   * @ngdoc function
   * @name udbApp.controller:MainCtrl
   * @description
   * # MainCtrl
   * Controller of the udbApp
   */
  angular
    .module('udbApp')
    .controller('MainCtrl', MainController);

  MainController.$inject = ['$scope', 'uitidAuth'];

  function MainController($scope, uitidAuth) {
    $scope.login = function () {
      uitidAuth.login();
    };

    $scope.$watch(function () {
      return uitidAuth.getUser();
    }, function (user) {
      $scope.user = user;
    }, true);
  }
})();
