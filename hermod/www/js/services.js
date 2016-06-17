angular.module('hermod.services', [])


.service("User", function() {
    
    this.username = null;
    this.token = null;

    this.setToken = function(new_token) {
        this.token = new_token;
    };
    this.getToken = function() {
        return this.token;
    };
    this.setUsername = function(new_username) {
        this.username = new_username;
    };
    this.getUsername = function() {
        return this.username;
    };
            
})

// Service to handle the check of credentials 

.service('LoginService', function($q, API, User, $http) {
    return {
        loginUser: function(username, password) {
            var deferred = $q.defer();
            var promise = deferred.promise;


            User.setUsername(username);

            $http.post(API.url + "/auth",
              {
                username: username,
                password: password
              }).
            then(function(data) {
              console.log(data);
                if (data.data.access_token) {
                  // Set token and headers
                  User.setToken(data.data.access_token);
                  var header = {
                    "Username": User.getUsername(),
                    "Authorization": "JWT " + User.getToken(),
                    "Content-Type":'application/json'
                  };
                  //$http.defaults.headers.common.Authorization = 'Basic YmVlcDpib29w';
                  $http.defaults.headers.common.Authorization = "JWT " + User.getToken();
                  // $httpProvider.defaults.headers.get = header;
                  // $httpProvider.defaults.headers.post = header;
                  deferred.resolve("Welcome");
                }
                else {
                  deferred.reject('Wrong credentials.');
                }
            });

            promise.success = function(fn) {
                promise.then(fn);
                return promise;
            };
            promise.error = function(fn) {
                promise.then(null, fn);
                return promise;
            };
            return promise; 
        },

        mockLoginUser: function(username, password) {

          var deferred = $q.defer();
          var promise = deferred.promise;

          if (username.toLowerCase() == "usuario" && password.toLowerCase() == "senha") {
              deferred.resolve('Welcome ' + name + '!');
          } else {
              deferred.reject('Wrong credentials.');
          }

          promise.success = function(fn) {
              promise.then(fn);
              return promise;
          };
          promise.error = function(fn) {
              promise.then(null, fn);
              return promise;
          };
          return promise;
        },

        autoLogin: function(username, password) {

          var deferred = $q.defer();
          var promise = deferred.promise;

          deferred.resolve('Welcome ' + name + '!');

          promise.success = function(fn) {
              promise.then(fn);
              return promise;
          };
          promise.error = function(fn) {
              promise.then(null, fn);
              return promise;
          };
          return promise;
        }
    };
})

.service('RegisterService', function($q, $http, API) {
    return {
        registerUser: function(data) {
            var deferred = $q.defer();
            var promise = deferred.promise;

            if (data.password != data.confirmpassword) {
              deferred.reject("Please make sure your passwords match.");
            }
            else {
              $http.post(API.url + "/api/register", data)
              .success(function(data) {
                console.log(data);
                  if (data.success) {
                    deferred.resolve('Welcome ' + name + '!');
                  }
                  else {
                    deferred.reject(data.error);
                  }
              });
            }

            promise.success = function(fn) {
                promise.then(fn);
                return promise;
            };
            promise.error = function(fn) {
                promise.then(null, fn);
                return promise;
            };
            return promise; 
        },

        mockRegisterUser: function(data) {

          var deferred = $q.defer();
          var promise = deferred.promise;

          deferred.resolve('Welcome ' + name + '!');

          promise.success = function(fn) {
              promise.then(fn);
              return promise;
          };
          promise.error = function(fn) {
              promise.then(null, fn);
              return promise;
          };
          return promise;
        }
    };
})

// Mock for the challenges info

.service('ChallengesMock', function($q) {
    return {
        getRank: function(challengeName) {
          return [
          {
            sequence: 1,
            username: 'lucasmullerm',
            rating: '8001 ± 500'
          },
          {
            sequence: 2,
            username: 'gabrielilharco',
            rating: '7000 ± 500'
          }];
        }
    };
})

// Mock for the news info

.service('NewsMock', function($q) {
    return {
        getNews: function() {
          return [
          {
            "author":"vyrp",
            "content":"Start playing this ancient game of tactics and moving pieces.<br>\r\nWrite a solution, and challenge the masters!\r\n\r\n<a href=\"http://mjollnir.rro.im/challenge/Backgammon\">Backgammon</a> is now available for submissions!",
            "datetime":"Thu, 29 Oct 2015 09:52:07 GMT",
            "title":"New Challenge: Backgammon!"
          },
          {
            "author":"vyrp",
            "content":"Enter a dangerous cave, fight a terrible monster and come back with glory and a treasure!\r\n\r\nThe <a href=\"/challenge/Wumpus\">World of Wumpus</a> challenge is online, and you may now submit a fearless solution!",
            "datetime":"Thu, 22 Oct 2015 03:26:33 GMT",
            "title":"New Challenge: Wumpus!"
          }];
        }
    };
});







