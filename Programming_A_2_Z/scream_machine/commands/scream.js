import { SlashCommandBuilder } from "discord.js";

export const data = new SlashCommandBuilder()
  .setName("scream")
  .setDescription("Replies to scream!")
  .addStringOption((option) =>
    option.setName("keywords").setDescription("What to scream")
  );

export async function execute(interaction) {
  await interaction.deferReply();

  let defaultScream = "ahhhhhhhhhhhhhhhhhhhh!";

  console.log(interaction.options.getString("keywords"));

  let screamPhrase = interaction.options.getString("keywords") ?? defaultScream;

  await interaction.followUp({
    content: screamPhrase.toUpperCase(),
  });
}
