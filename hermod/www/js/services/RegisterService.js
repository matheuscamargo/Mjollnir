servicesModule

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
});