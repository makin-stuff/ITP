import { SlashCommandBuilder } from "discord.js";

export const data = new SlashCommandBuilder()
  .setName("scared")
  .setDescription("Replies to scared");

export async function execute(interaction) {
  await interaction.reply("I'm scared!");
}
