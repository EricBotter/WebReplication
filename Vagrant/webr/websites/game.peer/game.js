//Dichiarazione "classe" Point
function Point(x,y){
    this.x = x;
    this.y = y;
    return this;
}

//Dichiarazione texture
var textures = {
    gun1 : PIXI.Texture.fromImage("gun1.png"),
    gun2 : PIXI.Texture.fromImage("gun2.png"),
    stand : PIXI.Texture.fromImage("stand.png"),
    sand : PIXI.Texture.fromImage("sand.png"),
    bullet : PIXI.Texture.fromImage("bullet.png"),
    ball : PIXI.Texture.fromImage("ball.png"),
    ufo : PIXI.Texture.fromImage("ufo.png"),
    missile: PIXI.Texture.fromImage("missile.png"),
    exp1 : PIXI.Texture.fromImage("exp1.png"),
    exp2 : PIXI.Texture.fromImage("exp2.png"),
    null : PIXI.Texture.fromImage("null.png")
}

//Dichiarazione oggetti di gioco
function Gun(spawnpoint, ammo, firingRate){
    PIXI.Sprite.call(this, textures.gun1);
    this.firing = false;
    this.position = spawnpoint;
    this.ammo = ammo;
    this.firingRate = firingRate;
    this.anchor = new Point(0.4, 0.5);
    this.shotsOnTarget = 0
    this.hyperTimer = 0;
}
Gun.prototype = PIXI.Sprite.prototype;

function Enemy(texture, spawnpoint, life, advance, die){
    PIXI.Sprite.call(this, texture);
    this.position = spawnpoint;
    this.life = life;
    this.advance = advance;
    this.die = die;
    this.anchor = new Point(0.5, 0.5);
    this.text = new PIXI.Text(life, {font:"20px Arial",fill:"#FFFFFF"});
    this.text.position = this.position;
    this.text.anchor = new Point(0.5, 0.5);
}
Enemy.prototype = PIXI.Sprite.prototype;

function Ufo(spawnpoint, life, speed){
    this.speed = speed;
    Enemy.call(this, textures.ufo, spawnpoint, life,
        function(ms){
            this.position.x -= this.speed * ms;
            if (this.position.x < -50)
                this.die(false);
        },
        function(killed){
            if (killed){
                Game.dying.push(this);
                this.text.setText('');
                this['counter'] = 2;
                ++Game.enemiesHit;
            } else {
                ++Game.enemiesMiss;
                Game.consecutives = 0;
            }
            Game.removeEnemy(this);
        }
    );
}
Ufo.prototype = Enemy.prototype;

function Missile(spawnpoint, life, speed){
    this.speed = speed;
    Enemy.call(this, textures.missile, spawnpoint, life,
        function(ms){
            this.position.y += this.speed * ms;
            if (this.position.y > renderer.height-100)
                this.die(false);
        },
        function(killed){
            if (killed){
                Game.dying.push(this);
                this.text.setText('');
                this['counter'] = 2;
                ++Game.enemiesHit;
            } else {
                ++Game.enemiesMiss;
                Game.consecutives = 0;
                Game.lose('A missile hit the ground.');
                Game.dying.push(this);
                this.text.setText('');
                this['counter'] = 1.8E+10308; //Infinity
            }
            Game.removeEnemy(this);
        }
    );
    this.text.setStyle({font:"20px Arial",fill:"#000000"});
    this.text.anchor.y = -1.0;
}
Missile.prototype = Enemy.prototype;

function Bullet(spawnpoint, rotation, damage, isHyper){
    PIXI.Sprite.call(this, textures.bullet);
    this.position = spawnpoint;
    this.rotation = rotation;
    if (isHyper)
    {
        this.scale = new Point(2.0, 2.0);
        this.damage = 100 * damage;
    }
    else
        this.damage = damage;
}
Bullet.prototype = PIXI.Sprite.prototype;

var Game = {
    lost: false,
    enemiesHit: 0,
    enemiesMiss: 0,
    consecutives: 0,
    bullets: [],
    enemies: [],
    dying: [],
    spawners: [],
    addEnemy: function(enemy){
        this.enemies.push(enemy);
        stage.addChild(enemy);
        stage.addChild(enemy.text);
    },
    removeEnemy: function(enemy){
        this.enemies.splice(this.enemies.indexOf(enemy), 1);
    },
    addBullet: function(bullet){
        this.bullets.push(bullet);
        stage.addChild(bullet);
    },
    removeBullet: function(bullet){
        bullet.visible = false;
        this.bullets.splice(this.bullets.indexOf(bullet), 1);
        stage.removeChild(bullet);
    },
    removeBulletAt: function(index){
        Game.bullets[index].visible = false;
        this.bullets.splice(index, 1);
    },
    lose: function(text){
        if (!this.lost){
            for (var i=0; i<this.spawners.length; ++i)
                window.clearInterval(this.spawners[i]);
            if (confirm("You lost!\n" +text+"\nPress OK to restart the game."))
                document.location.reload();
            this.lost = true;
        }
    }
}
