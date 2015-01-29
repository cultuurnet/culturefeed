(function () {
  'use strict';

  /**
   * @ngdoc function
   * @name udbApp.controller:NewEventCtrl
   * @description
   * # NewEventCtrl
   * udbApp controller
   */
  angular
    .module('udbApp')
    .constant('moment', moment)
    .controller('NewEventCtrl', NewEventCtrl);

  NewEventCtrl.$inject = ['udbApi', '$scope', '$q', '$http', 'appConfig', 'moment'];

  function NewEventCtrl(udbApi, $scope, $q, $http, appConfig, moment) {
    var working = false;
    $scope.newEventUrl = undefined;
    $scope.busy = function () {
      return working;
    };

    udbApi.newEvent = function (name, location, date) {
      var deferred = $q.defer();
      $http.post(
        appConfig.baseUrl + 'events',
        {
          name: name,
          location: location,
          date: moment(date).format('YYYY-MM-DDTHH:mm:ssZ')
        },
        {
          withCredentials: true,
          headers: {
            'Content-Type': 'application/json'
          }
        }
      ).success(function (data) {
          deferred.resolve(data);
        })
        .error(function (data) {
          deferred.reject(data.error);
        });

      return deferred.promise;
    };

    $scope.newEvent = function (name, location, date) {
      $scope.newEventUrl = undefined;
      working = true;
      udbApi
        .newEvent(name, location, date)
        .then(function (data) {
          $scope.newEventUrl = data.url;

          $scope.name = undefined;
          $scope.date = undefined;
        }, function (error) {
          window.alert(error);
        })
        .finally(function () {
          working = false;
        });
    };
  }

})();
