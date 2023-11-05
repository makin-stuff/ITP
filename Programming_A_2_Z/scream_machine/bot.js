import dotenv from "dotenv";
import { Client, Events, GatewayIntentBits } from "discord.js";
import * as scream from "./commands/scream.js";

console.log("Beep boop!");
dotenv.config();

// Create a new client instance
const client = new Client({
  intents: [GatewayIntentBits.Guilds],
});

// When the client is ready, run this code (only once)
client.once(Events.ClientReady, readyDiscord);

client.login(process.env.TOKEN);

function readyDiscord(c) {
  console.log("peepee logged in as " + c.user.tag);
}

client.on("interactionCreate", handleInteraction);

async function handleInteraction(interaction) {
  if (!interaction.isCommand()) {
    return;
  }

  if (interaction.commandName === "scream") {
    await scream.execute(interaction);
  }
}
