servicesModule

.service('LoginService', function($q, API, $http, $localStorage) {
    return {
        loginUser: function(username, password) {
            var deferred = $q.defer();
            var promise = deferred.promise;


            //User.setUsername(username);
            $localStorage.username = username;

            $http.post(API.url + "/auth",
              {
                username: username,
                password: password
              }).
            then(function(res) {
              //console.log(res);
                if (res.data.access_token) {
                  // Set token and headers
                  // User.setToken(res.data.access_token);
                  $localStorage.token = res.data.access_token;
                  // $http.defaults.headers.common.Authorization = "JWT " + User.getToken();
                  $http.defaults.headers.common.Authorization = "JWT " + $localStorage.token;
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
});