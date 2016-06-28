servicesModule


// Mock for the news info

.service('NewsMock', function($q) {
    return {
        getNews: function() {
          return [
          {
            "author":"Mock User",
            "content":"Start playing this ancient game of tactics and moving pieces.<br>\r\nWrite a solution, and challenge the masters!\r\n\r\n<a href=\"http://mjollnir.rro.im/challenge/Backgammon\">Backgammon</a> is now available for submissions!",
            "datetime":"Thu, 29 Oct 2015 09:52:07 GMT",
            "title":"New Challenge: Backgammon!"
          },
          {
            "author":"Mock User",
            "content":"Enter a dangerous cave, fight a terrible monster and come back with glory and a treasure!\r\n\r\nThe <a href=\"/challenge/Wumpus\">World of Wumpus</a> challenge is online, and you may now submit a fearless solution!",
            "datetime":"Thu, 22 Oct 2015 03:26:33 GMT",
            "title":"New Challenge: Wumpus!"
          }];
        }
    };
});