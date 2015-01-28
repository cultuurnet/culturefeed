(function(settings) {
  'use strict';

  /**
   * @ngdoc overview
   * @name udbApp
   * @description
   * # udbApp
   *
   * Main module of the application.
   */
  angular
    .module('udbApp', [
      'ngAnimate',
      'ngCookies',
      'ngSanitize',
      'udb.core'
    ])
    .config(udbAppConfig)
    .run(function (udbApi) {
      udbApi.getMe();
    })
    .constant('appConfig', settings.appConfig);

  udbAppConfig.$inject = ['$sceDelegateProvider', '$translateProvider', 'uiSelectConfig', 'appConfig',
    'queryFieldTranslations', 'dutchTranslations'];

  function udbAppConfig($sceDelegateProvider, $translateProvider, uiSelectConfig, appConfig,
                        queryFieldTranslations, dutchTranslations) {

    //$locationProvider.html5Mode(true);

    $sceDelegateProvider.resourceUrlWhitelist([
      'self',
      appConfig.baseUrl + '**'
    ]);

    // Translation configuration
    var defaultTranslations = _.merge(dutchTranslations, queryFieldTranslations.nl);

    $translateProvider
      .translations('nl', defaultTranslations)
      .preferredLanguage('nl');
    // end of translation configuration

    uiSelectConfig.theme = 'bootstrap';
  }
})(drupalSettings.culturefeed_udb3_app);
