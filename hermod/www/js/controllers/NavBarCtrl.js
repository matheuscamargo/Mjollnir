controllerModule

.controller('NavBarCtrl', function($scope, $state, $localStorage) {
    $scope.logout = function() {
      $localStorage.token = undefined;
      $state.go('login');
    };
});