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
          deferred.resolve(data.eventId);
        })
        .error(function (data) {
          deferred.reject(data.error);
        });

      return deferred.promise;
    };

    $scope.newEvent = function (name, location, date) {
      console.log(name);
      console.log(location);
      console.log(date);

      udbApi.newEvent(name, location, date)
        .then(function (eventId) {
          window.alert(eventId);
        },
        function (error) {
          window.alert(error);
        });
    };
  }

})();
