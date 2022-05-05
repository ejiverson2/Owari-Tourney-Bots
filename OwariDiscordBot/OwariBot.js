const Discord = require("Discord.js");
const { token } = require("./config.json");
var Owari = require("./Owari.js");

var game;
var gameEmbed;
var northPitMap = new Array(5, 4, 3, 2, 1, 0);

const client = new Discord.Client({ intents: [Discord.Intents.FLAGS.GUILDS, Discord.Intents.FLAGS.GUILD_MESSAGES, Discord.Intents.FLAGS.GUILD_MESSAGE_REACTIONS] });
client.login(token);

client.on('ready', () => {
    console.log("Bot's ready.");
})

client.on("messageCreate", async (msg) => {
    if (msg.author.bot) return;
    //console.log(msg.content);
    if (msg.content.toLowerCase().startsWith("play owari")) {
        if (game == null) {
            game = new Owari();
            let embed = new Discord.MessageEmbed()
                .setColor("#00FF00")
                .setTitle("First player(south), pick a starting move.")
                .setDescription(game.boardToString());
            let localGameEmbed = await msg.channel.send({ embeds: [embed] });
            //var localGameMessage = await msg.channel.send("First player(south), pick a starting move.\n" + game.boardToString())
            await localGameEmbed.react('1️⃣');
            await localGameEmbed.react('2️⃣');
            await localGameEmbed.react('3️⃣');
            await localGameEmbed.react('4️⃣');
            await localGameEmbed.react('5️⃣');
            await localGameEmbed.react('6️⃣');
            gameEmbed = localGameEmbed;
        } else {
            msg.channel.send("Game is already in progress")
        }
    }
})

client.on("messageReactionAdd", async (reaction, user) => {
    if (reaction.message.partial) await reaction.message.fetch();
    if (reaction.partial) await reaction.fetch();
    if (user.bot) return;
    reaction.users.remove(user.id);

    if (gameEmbed != null && reaction.message.id === gameEmbed.id) {
        let selection;
        switch (reaction.emoji.name) {
            case '1️⃣':
                selection = 0;
                break;
            case '2️⃣':
                selection = 1;
                break;
            case '3️⃣':
                selection = 2;
                break;
            case '4️⃣':
                selection = 3;
                break;
            case '5️⃣':
                selection = 4;
                break;
            case '6️⃣':
                selection = 5;
                break;
            default:
                selection = -1;
        }
        if (game.turn === 1) {
            selection = northPitMap[selection];
        }
        console.log("Turn:" + game.turn.toString() + " Selection:" + selection.toString());
        var turnState = game.doTurn(selection);
        let title;
        if (turnState == 1) {
            var ended = game.checkWin();
            if (ended > 0) {
                title = "SOUTH WINS";
                let embed = new Discord.MessageEmbed()
                    .setColor("#FF00FF")
                    .setTitle(title)
                    .setDescription(game.boardToString());
                gameEmbed.edit({ embeds: [embed] });
                gameEmbed = null;
                game = null;
            } else if (ended < 0) {
                title = "NORTH WINS";
                let embed = new Discord.MessageEmbed()
                    .setColor("#FF0000")
                    .setTitle(title)
                    .setDescription(game.boardToString());
                gameEmbed.edit({ embeds: [embed] });
                gameEmbed = null;
                game = null;
            }
        }

        if (game.turn == 0) {
            title = "It's SOUTH's Turn.";
        } else {
            title = "It's NORTH's Turn.";
        }

        if (turnState === -1) {
            title += " (Pick a valid pit.)";
        }
        let embed = new Discord.MessageEmbed()
            .setColor("#00FF00")
            .setTitle(title)
            .setDescription(game.boardToString());
        gameEmbed.edit({ embeds: [embed] });
    }
})

// const readline = require("readline");
// const rl = readline.createInterface({
//     input: process.stdin,
//     output: process.stdout
// });

// rl.on("line", line => {
//     client.channels.cache.get("370786232931975174").send(line);
// });