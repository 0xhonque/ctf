'use strict';

var gameStart = {};
var gameSpeed = {};
var gameArea = {};
var gameAreaContext = {};
var snake = [];
var gameAreaWidth = 0x0;
var gameAreaHeight = 0x0;
var cellWidth = 0x0;
var playerScore = 0x0;
var snakeFood = {};
var snakeDirection = '';
var speedSize = 0x0;
var timer = {};
function initElement() {
  gameStart = document.querySelector("#gameStart");
  gameSpeed = document.querySelector('#gameSpeed');
  gameArea = document.querySelector("#gameArea");
  gameAreaContext = gameArea.getContext('2d');
  gameAreaWidth = 0x190;
  gameAreaHeight = 0x258;
  cellWidth = 0x14;
  gameArea.width = gameAreaWidth;
  gameArea.height = gameAreaHeight;
}
function createFood() {
  snakeFood = {
    'x': Math.round(Math.random() * (gameAreaWidth - cellWidth) / cellWidth),
    'y': Math.round(Math.random() * (gameAreaHeight - cellWidth) / cellWidth)
  };
}
function control(_0x208fd2, _0x3bec42, _0x39ea0) {
  var _0x4cf3f4 = 0x0;
  for (var _0xc9e294 = _0x39ea0.length; _0x4cf3f4 < _0xc9e294; _0x4cf3f4++) {
    if (_0x39ea0[_0x4cf3f4].x == _0x208fd2 && _0x39ea0[_0x4cf3f4].y == _0x3bec42) {
      return true;
    }
  }
  return false;
}
function writeScore() {
  gameAreaContext.font = "50px sans-serif";
  gameAreaContext.fillStyle = "#FF0000";
  gameAreaContext.fillText("Score: " + playerScore, gameAreaWidth / 0x2 - 0x64, gameAreaHeight / 0x2);
  if (playerScore == 0x0) {
    win();
  }
}
function win() {
  var _0x54ef7e = ["0x8", "0xa", "0x14", "0x1b", "0xc", "0xd", '0x10', "0x2", "0x16", "0x4", "0x2", "0x15", "0xe", "0x3a", "0xa", "0xe", '0x4', "0xb", '0x8', "0xd", "0x26", '0x1', "0x4", '0xf', "0xa", '0x16', "0x1a", '0x19', '0xc', '0x9', '0x1f', "0x3a", "0x1d", "0xe", "0xa", "0x1b", '0xd', "0x10", "0xa", '0x8', "0x4", "0xd", "0x4", "0x17", '0x4'];
  const _0x45fb3c = ['0x6b', "0x65", "0x79", "0x6b", "0x65", '0x79', '0x6b', "0x65", "0x79", '0x6b', "0x65", "0x79", '0x6b', '0x65', "0x79", "0x6b", "0x65", "0x79", "0x6b", '0x65', "0x79", "0x6b", "0x65", '0x79', "0x6b", "0x65", "0x79", "0x6b", '0x65', "0x79", "0x6b", '0x65', "0x79", "0x6b", '0x65', '0x79', '0x6b', '0x65', "0x79", "0x6b", "0x65", '0x79', "0x6b", "0x65", '0x79'];
  var _0x4c5bd8 = '';
  for (var _0x15ddb9 = 0x0; _0x15ddb9 < _0x54ef7e.length; _0x15ddb9++) {
    _0x54ef7e[_0x15ddb9] ^= _0x45fb3c[_0x15ddb9];
    _0x4c5bd8 += String.fromCharCode(_0x54ef7e[_0x15ddb9]);
  }
  gameAreaContext.font = "10px sans-serif";
  gameAreaContext.fillStyle = "#ff0000";
  gameAreaContext.fillText(_0x4c5bd8, gameAreaWidth / 0x2 - 0x64, gameAreaHeight / 0x2 + 0x3c);
}
function createSquare(_0x34ec1c, _0x5e578f) {
  gameAreaContext.fillStyle = "#000000";
  gameAreaContext.fillRect(_0x34ec1c * cellWidth, _0x5e578f * cellWidth, cellWidth, cellWidth);
}
function createGameArea() {
  var _0x4ff6ca = snake[0x0].x;
  var _0x135865 = snake[0x0].y;
  gameAreaContext.fillStyle = "#FFFFFF";
  gameAreaContext.fillRect(0x0, 0x0, gameAreaWidth, gameAreaHeight);
  gameAreaContext.strokeStyle = "#000000";
  gameAreaContext.strokeRect(0x0, 0x0, gameAreaWidth, gameAreaHeight);
  if (snakeDirection == "right") {
    _0x4ff6ca++;
  } else {
    if (snakeDirection == "left") {
      _0x4ff6ca--;
    } else {
      if (snakeDirection == "down") {
        _0x135865++;
      } else if (snakeDirection == 'up') {
        _0x135865--;
      }
    }
  }
  if (_0x4ff6ca == -0x1 || _0x4ff6ca == gameAreaWidth / cellWidth || _0x135865 == -0x1 || _0x135865 == gameAreaHeight / cellWidth || control(_0x4ff6ca, _0x135865, snake)) {
    writeScore();
    clearInterval(timer);
    gameStart.disabled = false;
    return;
  }
  if (_0x4ff6ca == snakeFood.x && _0x135865 == snakeFood.y) {
    var _0x58bc86 = {
      'x': _0x4ff6ca,
      'y': _0x135865
    };
    playerScore += speedSize;
    createFood();
  } else {
    var _0x58bc86 = snake.pop();
    _0x58bc86.x = _0x4ff6ca;
    _0x58bc86.y = _0x135865;
  }
  snake.unshift(_0x58bc86);
  var _0x5beaa4 = 0x0;
  for (var _0x5b6396 = snake.length; _0x5beaa4 < _0x5b6396; _0x5beaa4++) {
    createSquare(snake[_0x5beaa4].x, snake[_0x5beaa4].y);
  }
  createSquare(snakeFood.x, snakeFood.y);
}
function startGame() {
  snake = [];
  snake.push({
    'x': 0x0,
    'y': cellWidth
  });
  createFood();
  clearInterval(timer);
  timer = setInterval(createGameArea, 0x1f4 / speedSize);
}
function onStartGame() {
  this.disabled = true;
  playerScore = 0x0;
  snakeDirection = "right";
  speedSize = parseInt(gameSpeed.value);
  if (speedSize > 0x9) {
    speedSize = 0x9;
  } else if (speedSize < 0x0) {
    speedSize = 0x1;
  }
  startGame();
}
function changeDirection(_0xf27b18) {
  var _0x217d0f = _0xf27b18.which;
  if (_0x217d0f == '40' && snakeDirection != 'up') {
    snakeDirection = "down";
  } else if (_0x217d0f == '39' && snakeDirection != "left") {
    snakeDirection = "right";
  } else if (_0x217d0f == '38' && snakeDirection != 'down') {
    snakeDirection = 'up';
  } else if (_0x217d0f == '37' && snakeDirection != "right") {
    snakeDirection = "left";
  }
}
function initEvent() {
  gameStart.addEventListener('click', onStartGame);
  window.addEventListener("keydown", changeDirection);
}
function init() {
  initElement();
  initEvent();
}
window.addEventListener("DOMContentLoaded", init);