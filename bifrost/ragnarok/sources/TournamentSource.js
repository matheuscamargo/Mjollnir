import _ from 'underscore';

var mockData = {
  name: 'TOURNAMENT',
  players: ["Mut", "Peh", "Gafa", "Bay", "Philip", "Josu", "Caique", "Mil"],
  };

class TournamentSource {
  fetch() {
    return new Promise(function(resolve, reject) {
      setTimeout(function() {
        console.log(mockData);
        resolve(mockData);
      }, 250);
    });
  }

  playMatch(matchInfo) {
  	var x = {
          cid: matchInfo.challenge,
          ids: matchInfo.players,
          rounds: 1,
        };
  	console.log(x);
	return new Promise(function(resolve, reject) {
      // Simulating POST
      $.ajax({
        type: "POST",
        url: "/tournament/1/",
        data: {
          cid: matchInfo.challenge,
          ids: matchInfo.players,
          rounds: 1,
        },
        success: function(response) {
          console.log(response);
          resolve(response);
        },
      });
    });
  }

  getMatch(matchInfo) {
    return new Promise(function(resolve, reject) {
      // Simulating GET request to find desired matchId
      $.ajax({
        type: "GET",
        url: "/tournament/1/match/" + matchId,
        success: function(response) {
          console.log(response);
          resolve(response);
        },
      });
    });
  }
}

export default new TournamentSource();
