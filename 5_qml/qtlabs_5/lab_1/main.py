from peewee import Model, SqliteDatabase
from peewee import CharField, IntegerField, ForeignKeyField

db_games = SqliteDatabase('games.db')


class BaseModel(Model):
    class Meta:
        database = db_games


class Game(BaseModel):
    name = CharField()
    online = IntegerField()
    publisher = CharField()


class Player(BaseModel):
    nickname = CharField()
    current_game = ForeignKeyField(Game)
    hours_count = IntegerField()


class GamesDB():
    def __init__(self, db: SqliteDatabase):
        self.db = db
        self.create_tables()

    def create_tables(self) -> None:
        self.db.drop_tables([Game, Player])
        self.db.create_tables([Game, Player])

    def insert_game(self, name: str, online: int, publisher: str) -> None:
        game = Game(name=name, online=online, publisher=publisher)
        game.save()

    def insert_player(self, nickname: str, current_game_name: str, hours_count: int) -> None:
        game = Game.select().where(Game.name=='Minecraft')
        player = Player(nickname=nickname, current_game = game, hours_count=hours_count)
        player.save()

    def select_games(self):
        return Game.select()
    
    def select_players(self):
        return Player.select()


orm = GamesDB(db_games)
orm.create_tables()

orm.insert_game('Minecraft', 1000000, 'Mojang Studios')
orm.insert_game('CS 2', 500000, 'Valve')
orm.insert_game('League of Legends', 3000000, 'Riot Games')
orm.insert_game('World of Warcraft', 200000, 'Blizzard Entertaiment')

orm.insert_player('Vpert', 'League of Legends', 5000)
orm.insert_player('Faker', 'League of Legends', 30000)
orm.insert_player('Timggg', 'CS 2', 1000)
orm.insert_player('wolfdemetrio', 'Minecraft', 200)
orm.insert_player('Garchomprokz', 'World of Warcraft', 40000)

print('________GAMES________')
for game in orm.select_games():
    print(f'{game.name} ({game.publisher})')

print('________PLAYERS________')
for player in orm.select_players():
    print(f'{player.nickname} -- {player.current_game.name} ({player.hours_count}h)')
