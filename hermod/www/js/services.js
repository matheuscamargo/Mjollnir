angular.module('hermod.services', [])

// Service to handle the check of credentials 

.service('LoginService', function($q, $http, SERVER) {
    return {
        loginUser: function(username, password) {
            var deferred = $q.defer();
            var promise = deferred.promise;

            // TODO: persistent login

            $http.post(SERVER.url + "api/login",
              {
                username: username,
                password: password
              }).
            then(function(data) {
              console.log(data);
                if (data.data.success) {
                  deferred.resolve('Welcome ' + name + '!');
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

.service('RegisterService', function($q, $http, SERVER) {
    return {
        registerUser: function(data) {
            var deferred = $q.defer();
            var promise = deferred.promise;

            if (data.password != data.confirmpassword) {
              deferred.reject("Please make sure your passwords match.");
            }
            else {
              $http.post(SERVER.url + "api/register", data)
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
});


