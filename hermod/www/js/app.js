// Hermod App

// 'hermod' is the name of this angular module (also set in a <body> attribute in index.html)
// the 2nd parameter is an array of 'requires'
// 'hermod.services' is found in services.js
// 'hermod.controllers' is found in controllers.js
angular.module('hermod', ['ionic', 'hermod.controllers', 'hermod.services'])

.constant('SERVER', {
  // Url used to reach the server (Bifrost)
  url: 'http://192.168.0.28:5000/'
})

.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {
    // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
    // for form inputs)
    if (window.cordova && window.cordova.plugins && window.cordova.plugins.Keyboard) {
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
      cordova.plugins.Keyboard.disableScroll(true);

    }
    if (window.StatusBar) {
      // org.apache.cordova.statusbar required
      StatusBar.styleDefault();
    }
  });
})

.config(function($stateProvider, $urlRouterProvider, $httpProvider) {

  $httpProvider.defaults.useXDomain = true;
  delete $httpProvider.defaults.headers.common['X-Requested-With'];

  // Settting up the various states for Angular UI router.
  // Each state's controller can be found in controllers.js
  $stateProvider

  .state('challenges', {
    url: '/challenges',
    templateUrl: 'templates/challenges.html',
    controller: 'ChallengesCtrl'
    })

  .state('challenge-detail', {
    url: '/challenges/:challengeName',
    templateUrl: 'templates/challenge-detail.html',
    controller: 'ChallengeDetailCtrl'
  })
  
  .state('login', {
    url: '/login',
    templateUrl: 'templates/login.html',
    controller: 'LoginCtrl'
  });

  // if none of the above states are matched, use this as the fallback
  // TODO: new state to check if already logged in
  $urlRouterProvider.otherwise('/login');

});
