// Hermod App

// 'hermod' is the name of this angular module (also set in a <body> attribute in index.html)
// the 2nd parameter is an array of 'requires'
// 'hermod.services' is found in services.js
// 'hermod.controllers' is found in controllers.js
angular.module('hermod', 
  ['ionic', 
  'hermod.controllers', 
  'hermod.services', 
  'stormpath', 
  'stormpath.templates'])

.constant('API', {
  // Url used to reach the server (Bifrost)
  url: 'http://192.168.0.27:5000/'
})

.run(function($ionicPlatform, $stormpath) {

  $stormpath.uiRouter({
    loginState: 'login',
    defaultPostLoginState: 'tabs.news'
  });

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

.config(function($stateProvider, $urlRouterProvider, $httpProvider, API, STORMPATH_CONFIG) {

  $httpProvider.defaults.useXDomain = true;
  delete $httpProvider.defaults.headers.common['X-Requested-With'];


  STORMPATH_CONFIG.ENDPOINT_PREFIX = API.url;

  // Settting up the various states for Angular UI router.
  // Each state's controller can be found in controllers.js
  $stateProvider

  .state('tab', {
    url: "/tab",
    abstract: true,
    templateUrl: "templates/tabs.html"
  })
  
 .state('tab.news', {
    url: '/news',
    views: {
      'tab-news': {
        templateUrl: 'templates/tab-news.html',
        controller: 'NewsCtrl'
      }
    },
    sp: {
      authenticate: true
    }
  })

  .state('tab.challenges', {
    url: '/challenges',
    views: {
      'tab-challenges': {
        templateUrl: 'templates/tab-challenges.html',
        controller: 'ChallengesCtrl'
      }
    }
  })

  .state('tab.groups', {
    url: '/groups',
    views: {
      'tab-groups': {
        templateUrl: 'templates/tab-groups.html',
        controller: 'GroupsCtrl'
      }
    },
    sp: {
      authenticate: true
    }
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
  })

  .state('register', {
    url: '/register',
    templateUrl: 'templates/register.html',
    controller: 'RegisterCtrl'
  });

  // if none of the above states are matched, use this as the fallback
  // TODO: new state to check if already logged in
  // $urlRouterProvider.otherwise('/login');
  $urlRouterProvider.otherwise( function($injector, $location) {
      var $state = $injector.get("$state");
      $state.go("login"); //redirect to a 404 page
  });


});